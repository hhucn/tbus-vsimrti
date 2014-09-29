/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */
/**
 * Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#include "CDRQ.h"
#include "TbusQueueControlInfo.h"

namespace projekt {

// class defines the simple module with name CDRQ
Define_Module(CDRQ);

/**
 * default constructor
 */
CDRQ::CDRQ() {
	queue = new cPacketQueue();
	currSendHeadCDRQPacket = new SendHeadAndDeletePacket(kSTRING_CDRQ_SENDHEAD);
}

/**
 * default destructor
 */
CDRQ::~CDRQ() {
	delete queue;
	delete currentBbdelay;
	cancelAndDelete(currSendHeadCDRQPacket);
}

/**
 * default initializer
 */
void CDRQ::initialize() {
	//TODO: find the right initialization values
	currentBbdelay = new Bbdelay();
	signalBbdelay = registerSignal(kSIGNAL_CDRQ_BBDELAY);
	emit(signalBbdelay, currentBbdelay->bbdelay());
}

/**
 * handling incoming messages in the simple module
 * we will look for the kind of message (self message, unknown, ...) and act like it has to be
 *
 * @param msg, incoming cMessage*
 */
void CDRQ::handleMessage(cMessage* msg) {
	if (msg->isSelfMessage()) {
		SendHeadAndDeletePacket* sendHead = check_and_cast<SendHeadAndDeletePacket*>(msg);

		if (sendHead) {
			// dispatch head events, schedule new self msgs
			EV << getFullPath() << " handles SendHeadAndDeletePacket for " << sendHead->getDeletePacketWithName() << endl;
			dispatch();
		}

		// all checks failed, trop the packet
	} else {
		addPacketToQueue(msg);
	}
}

/**
 * add a packet to the queue
 *
 * @param packet to add
 * @param simTime() simulation time
 */
void CDRQ::addPacketToQueue(cMessage* msg) {
	bool isQueueEmpty = queue->isEmpty();
//
//	// manipulate tstamp, when packet is dropped
//	if (job->getDroppedSimulated()) {
//		job->setArrivalTimeForLogging(job->getSendingTimeForLogging());
//	} else {
//		job->setArrivalTimeForLogging(simTime());
//	}

	// add time for the earliest delivery
	if (bbDelayVector.size() > 0 && bbTimeVector.size() > 0) {
		simtime_t delay = calculateBackboneDelay();

		TbusQueueControlInfo* controlInfo = new TbusQueueControlInfo();
		controlInfo->setEarliestDelivery(simTime() + delay);

		msg->setControlInfo(controlInfo);
	} else {
		opp_error("Adding packet without Backbonedelay set!");
	}

//	queue->insert(msg);
//
//	// starting self messaging process when queue is empty
//	if (isQueueEmpty) {
//		//update the vectors
//		bbDelayVector.push_back(currentBbdelay);
//		bbTimeVector.push_back(simTime());
//
//		// schedule self message
//		scheduleNewSendHeadAndDeletePacket(simTime());
//	}
}

void CDRQ::scheduleNewSendHeadAndDeletePacket() {

}

/**
 * Will schedule a new self-message (which will send head of queue and pop it) with given delay
 * @param simTime(): actually simulation time
 * @param delay: delay, when the self-message will be started
 */
void CDRQ::scheduleNewSendHeadAndDeletePacket(simtime_t delay) {
	if (currSendHeadCDRQPacket->isScheduled()) {
		EV<< getFullPath() << " currSendHeadCDSQPacket is currently scheduled! SHOULD NOT HAPPEN";
	}
	// dropped package got no delay
	if (((MyPacket*)queue->front())->getDroppedSimulated()) {
		delay = SIMTIME_ZERO;
	}

	// set new simtimes
	currSendHeadCDRQPacket->setDeletePacketWithName(((MyPacket*)queue->front())->getName());
	currSendHeadCDRQPacket->setSendingTimeForTesting(simTime());
	currSendHeadCDRQPacket->setArrivalTimeForTesting(simTime()+delay);

	// vorher könnten bbdelay usw. 0 sein
	((MyPacket*)queue->front())->setEarliestTimeForDelivery(simTime() + delay);

	// output in tkenv
	// set datarate for logging
	Bbdelay tmp = *currentBbdelay;
	((MyPacket*) queue->front())->setBbdelay(tmp);
	// send self msg
	scheduleAt(simTime() + delay, currSendHeadCDRQPacket);
	EV << getFullPath() << " scheduled SendHeadAndDeletePacket with d_bb: " << delay << endl;
}

/**
 * react on changes of the underlying network (due to client movement)
 * @param newDatarate
 * @param simTime()
 */
void CDRQ::bbDelayChanged(Bbdelay* newBbdelay) {
	//check if valid
	//if the new backbone is valid and changed in regards to the last we need to make changes
	bool delayChanged = false;

	if (newBbdelay->bbdelay() != currentBbdelay->bbdelay()) {
		//save the newBbdelay
		currentBbdelay = newBbdelay;
		delayChanged = true;

		//if queue is not empty we need to recalculate capacity and change the backbone vector
		if (!queue->empty()) {

			//add the current values to the vectors
			bbDelayVector.push_back(currentBbdelay);
			bbTimeVector.push_back(simTime());
		}
	}

//	if (queue->isEmpty() && delayChanged) {
//		currentBbdelay->setBbdelaySim(newBbdelay->bbdelay());
//	}

//	// output in tkenv and manipulating self messages, cause we got new delays
	if (delayChanged) {
//		char c[50];
//		currentBbdelay->toString(c);
//		EV<< getFullPath() << " backbone delay changed to " << c << " at " << simTime() <<endl;

		manipulateSelfMessageProcess();
		emit(signalBbdelay, currentBbdelay->bbdelay());
	}
}

/**
 * cancels current self message, which simulates backbone delay, when scheduled and backbone was changed.
 * even schedules new self message or calls dispatch directly
 */
void CDRQ::manipulateSelfMessageProcess() {
	if ((currSendHeadCDRQPacket->isScheduled() && !queue->isEmpty())) {
		cancelEvent(currSendHeadCDRQPacket);

		// get delay
		simtime_t delay = calculateRestBackboneDelay((MyPacket*) queue->front());

		// is delay valid?
		if (delay < 0) {
			delay = SIMTIME_ZERO;
		}

		// set new earliest time for delivery
		((MyPacket*) queue->front())->setEarliestTimeForDelivery(simTime() + delay);

		// we waited longer than needed OR there is some time to wait
		scheduleNewSendHeadAndDeletePacket(delay);

		for (int i = 0; i < queue->length(); i++) {
			simtime_t newDelay = calculateRestBackboneDelay((MyPacket*) queue->get(i));
			((MyPacket*) queue->get(i))->setEarliestTimeForDelivery(simTime() + newDelay);
		}
	}
}

/**
 * calculates and return current backbone delay in addiction to the past delays/times
 * @param simTime()
 */
simtime_t CDRQ::calculateBackboneDelay() {
	simtime_t runtime = simTime() - bbTimeVector.front();
	simtime_t bbDelay = SIMTIME_ZERO;
	simtime_t endtime = simTime();
	simtime_t starttime;

	// if runtime equals 0, we return the currentDatarate
	if (runtime == SIMTIME_ZERO) {
		return currentBbdelay->bbdelay();
	}

	// add all delays
	for (int i = bbDelayVector.size() - 1; i >= 0; i--) {
		starttime = bbTimeVector.at(i).dbl();
		bbDelay += bbDelayVector.at(i)->bbdelay().dbl() * (endtime - starttime) / runtime;
		// for debugging
//			EV << "\t" << i <<"  " << bbDelayVector.at(i)->bbdelay() << " * ( " << endtime << " - " << starttime << ") / " << runtime
//					<< "\t = " << (bbDelayVector.at(i)->bbdelay().dbl() * (endtime - starttime) / runtime) << endl;
		endtime = starttime;
	}

	return bbDelay;
}

/**
 * calculates and return current backbone delay in addiction to the past delays/times
 * minus the time, which the packet already waited
 * @param job MyPacket*
 * @param simTime() simulation time
 */
simtime_t CDRQ::calculateRestBackboneDelay(MyPacket* job) {
	// we waited since MyPacket arrived
	simtime_t delayGone = simTime() - job->getArrivalTimeForLogging();
	// but, now we have to wait....
	simtime_t delayNew = calculateBackboneDelay();
	// so there is just a little delay at the end
	simtime_t delayWait = delayNew - delayGone;
	return delayWait < SIMTIME_ZERO ? SIMTIME_ZERO : delayWait;
}

/**
 * this function handles the transmitting of the head packet and the loss of
 * packets The function first checks if there is enough capacity to send the
 * packet
 */
void CDRQ::dispatch() {
	// send head, if queue is not empty
	if (!queue->isEmpty()) {
		MyPacket* head = (MyPacket*) queue->pop();

		// add timestamp
		head->setTimestampsArraySize((1 + head->getTimestampsArraySize()));
		char timelog[50];
		sprintf(timelog, "%s %s %f", kSTRING_CDRQ, kSTRING_SEND, simTime().dbl());
		head->setTimestamps((head->getTimestampsArraySize() - 1), timelog);
		head->setSCDRQ(simTime());

		// reset earliest time for delivery
		head->setEarliestTimeForDelivery(SIMTIME_ZERO);

		// add logging send time
		if (!head->getDroppedSimulated())
			head->setSendingTimeForLogging(simTime());


		EV << getFullPath() << " sends " << head << endl;
		sendDelayed(head, SIMTIME_ZERO, "out");

		// self messaging process
		if (!queue->isEmpty()) {
			if (((MyPacket*) queue->front())->getEarliestTimeForDelivery() < simTime()) {
				dispatch();
			} else {
				scheduleNewSendHeadAndDeletePacket(simTime());
			}
		}
	}

	bbDelayVector.clear();
	bbTimeVector.clear();
	if (!queue->isEmpty()) {
		bbDelayVector.push_back(currentBbdelay);
		bbTimeVector.push_back(simTime());
	}
}

};

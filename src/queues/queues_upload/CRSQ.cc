/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */
/**
 * Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#include "CRSQ.h"

namespace projekt {

// this class defines the simple module with name CRSQ
Define_Module(CRSQ)
;

/**
 * default constructor; set test mode to false and init variabeles
 */
CRSQ::CRSQ() {
	queue = new cPacketQueue();
	currSendHeadCRSQPacket = new SendHeadAndDeletePacket(kSTRING_CRSQ_SENDHEAD);
}

/**
 * default destructor; garbage collection
 */
CRSQ::~CRSQ() {
	delete queue;
	delete currentDatarate;
	cancelAndDelete(currSendHeadCRSQPacket);
}

/**
 * default initializer; init default loss prob and datarate
 * if not in test mode, we will init signals too
 */
void CRSQ::initialize() {
	freeBufferInByte = INT32_MAX;

	//we all dont like nullpointers
	//some initialization in Bytes value to overcome null pointer exceptions that happen at first dataratechange event
	currentLossProbability = 0.0;
	currentDatarate = new Datarate(300, true, 0.0, "DUMMY");
	currentDatarate->setDatarateSim(300);

	//signals for collecting data (statistics)
	signalDatarate				= registerSignal(kSIGNAL_CRSQ_DATARATE);
	signalLossprob				= registerSignal(kSIGNAL_CRSQ_LOSSPROB);
	signalLossprobRnd			= registerSignal(kSIGNAL_CRSQ_LOSSPROB_RND);
	signalPacketDatarateDelay	= registerSignal(kSIGNAL_CRSQ_PACKET_DATARATE_DELAY);
	signalDropMeasured			= registerSignal(kSIGNAL_CRSQ_DROPMEASURED);
	signalDropSimulated			= registerSignal(kSIGNAL_CRSQ_DROPSIMULATED);

	emit(signalDatarate, currentDatarate->datarateSim());
	emit(signalLossprob, currentLossProbability);
}

/**
 * handling incoming messages in the simple module
 * we will look for the kind of message (self message, unknown, ...) and act like it has to be
 * @params msg, cMessage*
 */
void CRSQ::handleMessage(cMessage* msg) {
	SendHeadAndDeletePacket* sendHead = dynamic_cast<SendHeadAndDeletePacket*>(msg);
	MyPacket* job = dynamic_cast<MyPacket*>(msg);

	// is message a control message?
	if (strcmp(msg->getFullName(), kSTRING_CONTROL_CRSQ) == 0) {
//		handleControlMessage(msg);

	// or send head event?
	} else if (sendHead != 0) {
		handleSendHeadAndDeletePacket(sendHead);

	// or a job ?
	} else if (job != 0) {
		addPacketToQueue(job);

	// no..drop it
	} else {
		EV<< getFullPath() << " unknown message: " << msg->getFullName()
		<< ", type (" << msg << "), at " << simTime()<< endl;
	}
}

/**
 * Handles SendHeadAndDeletePacket. Will calculate bbdelay for current head of queue,
 * call dispatch for the current head and schedule new SendHeadAndDeletePacket
 * @param simTime()
 * @param sendHead
 */
void CRSQ::handleSendHeadAndDeletePacket(SendHeadAndDeletePacket* sendHead) {
	// calculate d_bb and send packet to CDSQ
	EV<< getFullPath() << " handles SendHeadAndDeletePacket for " << sendHead->getDeletePacketWithName() << endl;

	// dispatch only, when quee is not empty
	if (!queue->isEmpty()) {
		dispatch(NULL);

		scheduleNewSendHeadAndDeletePacket(simTime());
	} else {
		EV<< getFullPath() << " gets 'SendHeadAndDelete', but queue is empty, " << " at " << simTime() << endl;
	}
}

/**
 * Will schedule a new self-message (which will send head of queue and pop it) with given delay
 * @param simTime(): actually simulation time
 * @param delay: delay, when the self-message will be started
 */
void CRSQ::scheduleNewSendHeadAndDeletePacket(simtime_t delay) {
	if (!queue->empty()) {

		// set new simtimes
		currSendHeadCRSQPacket->setDeletePacketWithName(((MyPacket*) queue->front())->getName());
		currSendHeadCRSQPacket->setArrivalTimeForTesting(simTime() + delay);
		currSendHeadCRSQPacket->setSendingTimeForTesting(simTime());

		// send self msg
		scheduleAt(simTime() + delay, currSendHeadCRSQPacket);
		EV<< getFullPath() << " scheduled SendHeadAndDeletePacket with d_air: " << delay << endl;
	}
}

/**
 * add packet to the queue
 * @param job
 * @param simTime()
 */
void CRSQ::addPacketToQueue(cMessage* msg) {
//	// add timestamp
//	job->setTimestampsArraySize((1 + job->getTimestampsArraySize()));
//	char timelog[50];
//	sprintf(timelog, "%s %s,%.0f", kSTRING_CRSQ, kSTRING_RECEIVED, simTime().dbl() * kMULTIPLIER_S_TO_NS);
//	job->setTimestamps((job->getTimestampsArraySize() - 1), timelog);
//	job->setDCRSQ(simTime());
//
//	if (job->getDroppedSimulated()) {
//		job->setArrivalTimeForLogging(job->getSendingTimeForLogging());
//	} else {
//		job->setArrivalTimeForLogging(simTime());
//	}
//
//	// log packet for datarate
//	if (job->getPacketNumber() == 0){
//		currentDatarate->setCMsgNumberOfProbePackets(job->getNumberOfProbePackets());
//		currentDatarate->setCMsgPayloadSize(job->getPayloadSize());
//	}
//
//	//if queue is empty, store some data
//	bool isQueueEmpty = queue->empty();
//	if (isQueueEmpty) {
//		//update the vectors
//		datarateVector.push_back(currentDatarate);
//		drTimeVector.push_back(simTime());
//
//		//set capacity to zero, because we had no transmitted data yet
//		capacityBytes = 0;
//	}
//
//	bool jobInserted = false;
//	// Add the packet to the queue if there is space left in the queue - if not, drop it.
//	if (freeBufferInByte - job->getPayloadSize() > 0) {
//		freeBufferInByte -= job->getPayloadSize();
//		queue->insert(job);
//		jobInserted = true;
//	} else {
//		EV<< getFullPath() << " dropped " << job << " (no free buffer)" << " at " << simTime() << endl;
//		delete(job);
//	}
//
//		// if queue was empty, we will start the self messaging process, that will send the heads
//	if (isQueueEmpty && jobInserted) {
//		scheduleNewSendHeadAndDeletePacket(simTime());
//	}
}

/**
 * calculate the loss probability for this packet on basis of time[] and perf[]
 * @param simTime() - the time the current head packet will be
 * transmitted or lost
 * @return droprate as double
 */
double CRSQ::calculateLossProbability() {
	//check if we really transmit the packet or if it is lost
	//calculate the median loss probability
	simtime_t runtime = (simTime() - drTimeVector.front()).dbl();
	double loss = 0.0;
	simtime_t endtime = simTime().dbl();
	simtime_t starttime;
	for (int i = datarateVector.size() - 1; i >= 0; i--) {
		starttime = drTimeVector.at(i).dbl();
		loss += (datarateVector.at(i)->droprate() * ((endtime - starttime) / runtime));
		endtime = starttime;
	}
	currentLossProbability = loss;

	return loss;
}

/**
 * Calculates the transmission delay d_air
 */
simtime_t CRSQ::calculateDatarateDelay(double packetsize) {
	simtime_t runtime = simTime() - drTimeVector.front();
	simtime_t datarateDelay = 0.0;
	simtime_t endtime = simTime();
	simtime_t starttime;
//	long double datarate, endt, startt, runt, delay, res;

	// if runtime equals 0, we return the currentDatarate
	if (runtime== SIMTIME_ZERO){
		return currentDatarate->datarateSim() == 0 ? kDEFAULTDELAY : packetsize / currentDatarate->datarateSim();
	}

	// adding all delays
	for (int i = datarateVector.size() - 1; i >= 0; i--) {
		starttime = drTimeVector.at(i).dbl();
//		datarate = datarateVector.at(i)->datarate();
		// TODO for untested logfiles
//		startt = starttime.dbl();
//		endt = endtime.dbl();
//		runt = runtime.dbl();
//		delay = datarate * (endt - startt) / runt;
//		res = delay + datarateDelay.dbl();

//		bool greaterAsMax = (1+res) > simTime().getMaxTime().dbl(); // adding puffer
//		datarateDelay = greaterAsMax ? simTime().getMaxTime() : res;
//		datarateDelay += delay;
		datarateDelay = datarateVector.at(i)->datarateSim() * (endtime - starttime) / runtime;
		endtime = starttime;
	}

	return datarateDelay == SIMTIME_ZERO ? kDEFAULTDELAY : packetsize / datarateDelay;
}

/**
 * react on changes of the underlying network (due to client movement)
 * @param newDatarate
 * @param simTime()
 */
void CRSQ::datarateChanged(Datarate* newDatarate) {
	bool datarateChanged = newDatarate->validdatarate();

	if (newDatarate->validdatarate() && (newDatarate->datarateSim() != currentDatarate->datarateSim())) {
		currentDatarate = newDatarate->copy();
		datarateChanged = true;
	} else if (newDatarate->modemMode().compare(currentDatarate->modemMode()) != 0
			|| newDatarate->droprate() != currentDatarate->droprate()) {
		//only modemmode or droprate changed - combine the data
//		currentDatarate = new Datarate(currentDatarate->datarate(), currentDatarate->validdatarate(),
//				newDatarate->droprate(), newDatarate->modemMode());
		currentDatarate->setDroprate(newDatarate->droprate());
		currentDatarate->setModemMode(newDatarate->modemMode());
		datarateChanged = true;
	}

	// set simulated datarate
//	if (datarateChanged){
//		currentDatarate->setDatarateSim(newDatarate->datarate());
//	}

	if (!queue->empty() && datarateChanged) {
		//calculate the number of bytes that went over the air since the last
		//transmitted packet or networkchange
		if (drTimeVector.size() == 0 || datarateVector.size() == 0) {
			capacityBytes = 0;
		} else {
			capacityBytes += (simTime() - drTimeVector.back()).dbl() * datarateVector.back()->datarateSim();
		}

		if (capacityBytes < 0) {
			EV<< getFullPath() << " capacity < 0  " << capacityBytes << " at " << simTime() << endl;
		}

			//add the current values to the vectors
		datarateVector.push_back(currentDatarate);
		drTimeVector.push_back(simTime());
		char c[50];
		currentDatarate->toString(c);
		EV<< getFullPath() << " datarate changed to " << c << " at " << simTime() <<endl;

		// if testmode is disabled, log to harddrive and manipulate self message process
		// because we have no delays
		manipulateSelfMessageProcess();
		emit(signalDatarate, currentDatarate->datarateSim());
		emit(signalLossprob, calculateLossProbability());
	}
}

/**
 * cancels current self message, which simulates datarate delay, when scheduled and datarate was changed.
 * even schedules new self message or calls dispatch directly
 * @param simTime()
 */
void CRSQ::manipulateSelfMessageProcess() {
	// catch errors
	if (currentDatarate->datarateSim() == 0){
		return;
	}

	if (currSendHeadCRSQPacket->isScheduled() && !queue->isEmpty()) {
		cancelEvent(currSendHeadCRSQPacket);

		// calculate the number of bytes that went over the air since the last transmitted packet or networkchange
		double bytesLeft = ((MyPacket*) queue->front())->getPayloadSize() - capacityBytes;

		// calculate delay for the left bytes
		simtime_t delayToWait = bytesLeft / currentDatarate->datarateSim();

		// we waited longer than needed OR there is some time to wait
		if (delayToWait < 0) {
			dispatch(NULL);
			currSendHeadCRSQPacket->setSendingTimeForTesting(simTime());
			currSendHeadCRSQPacket->setArrivalTimeForTesting(simTime());
		} else {
			scheduleNewSendHeadAndDeletePacket(delayToWait);
		}
	}
}

/**
 * this function handles the transmitting of the head packet and the loss of
 * packets The function first checks if there is enough capacity to send the packet
 * @param simTime()
 * @param retBool, [0] = everything executed normally, [1] = msgLost
 */
void CRSQ::dispatch(bool retBool[]) {
	double newcapacity;

	//if the called method doesnt need return booleans, we will create dummys
	if (retBool == NULL) {
		retBool = new bool[2];
	}

	if (!queue->empty()) {
		//calculate the number of bytes that went over the air since the last transmitted packet or networkchange
		double add = (simTime() - drTimeVector.back()).dbl() * datarateVector.back()->datarateSim();
		newcapacity = capacityBytes + add;

		//only transmit if we really have enough capacity for the packet
		// WE HAVE ENOUGH CAPACITY, because sendHeadAnd... had latency, so we got capacity :)
		double diff = newcapacity - ((MyPacket*) queue->front())->getPayloadSize();

		if (diff >= 0 || fabs(diff) < kEPS3) {
			//don't substract the heads.packetsize here, this will be done in transmitorloosehead
			capacityBytes = newcapacity;

			if (capacityBytes < DBL_MIN) {
				EV<< getFullPath() << " capacity < 0 in handleMessage " << capacityBytes << " at " << simTime() << endl;
			}
				//transmitOrLoose the head packet
			transmitOrLooseHead(retBool);

			if (retBool[0]) {
				//now we should have 0.0 <= capacityBytes <1.0
				if (capacityBytes > 1.0) {
					EV<< getFullPath() << " capacity > 1 after handleMessage  " << capacityBytes << " at " << simTime() << endl;
				}
			}
		} else {
			EV << getFullPath() << ".dispatch called without enough capacity! THIS SHOULD NEVER HAPPEN (or only in Tests)!\n" << endl;
			retBool[0] = false;
			retBool[1] = false;
		}
	} else {
		retBool[0] = false;
		retBool[1] = false;
	}
}

/**
 * this function handles the transmitting of the head packet and the loss of
 * packets The function first checks if there is enough capacity to send the
 * packet
 * @param simTime()
 * @param retBool, [0] = transmitOrLooseHead executed normally, [1] = msgLost
 */
void CRSQ::transmitOrLooseHead(bool retBool[]) {
	bool isQueueEmpty = queue->empty();
	bool msgLost = false;
	MyPacket* head = (MyPacket*) queue->pop();

	// add logging send time
	if (!head->getDroppedSimulated()) head->setSendingTimeForLogging(simTime());

	double diff = capacityBytes - head->getPayloadSize();
	double loss = calculateLossProbability();
	head->setSCRSQ(simTime());
	head->setDroprateQueue(loss);
	head->setDatarate(*currentDatarate);

	if (!isQueueEmpty && (diff >= 0 || fabs(diff) < kEPS3)) {
		//subsctract the used capacity from the available capacity
		//ignore rounding errors smaller than eps
		capacityBytes = (diff >= 0) ? diff : 0;

		//add free space to queue
		freeBufferInByte += head->getPayloadSize();

		//check if we really transmit the packet or if it is lost
		double random = dblrand();
		emit(signalLossprobRnd, random);

		// add timestamp
		head->setTimestampsArraySize((1 + head->getTimestampsArraySize()));
		char timelog[50];
		sprintf(timelog, "%s %s %f", kSTRING_CRSQ, kSTRING_SEND, simTime().dbl());
		head->setTimestamps((head->getTimestampsArraySize() - 1), timelog);

		//emitting signals
		emit(signalDropMeasured,(random<loss) ? 1 : 0);
		emit(signalDropSimulated,head->getDroppedMeasured() ? -1 : 0);

		if (random >= loss) {
			msgLost = false;

			EV<< getFullPath() << " send " << head << " at " << simTime() << endl;
			// sendDelayed cannot be called in test files (wrong module)
			sendDelayed(head, SIMTIME_ZERO, "out");
		} else {

			head->setDroppedSimulated(true);
			EV << getFullPath() << " lost " << head << " at " << simTime() << endl;
			sendDelayed(head, SIMTIME_ZERO, "out");

			msgLost = true;
		}

		if (queue->isEmpty()) {
			//as the queue is empty now we clear all stored capacity
			datarateVector.clear();
			drTimeVector.clear();
			capacityBytes = 0;
		} else {
			//the queue is not empty so we need to change some values
			datarateVector.clear();
			datarateVector.push_back(currentDatarate);
			drTimeVector.clear();
			drTimeVector.push_back(simTime());
		}
		retBool[0] = true;
	} else {
		retBool[0] = false;
	}
	retBool[1] = msgLost;
}

};

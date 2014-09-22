/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "CDRQ.h"

namespace projekt {

// class defines the simple module with name CDRQ
Define_Module(CDRQ)
;

/**
 * default constructor
 */
CDRQ::CDRQ() {
	isInTestMode = false;
	queue = new cPacketQueue();
	currSendHeadCDRQPacket = new SendHeadAndDeletePacket(kSTRING_CDRQ_SENDHEAD);
	logwriter = new LogFileWriter();
}

/**
 * custom constructor with boolean for test modes
 * in test modes all outputs are forbidden
 * @param testModeEnabled
 */
CDRQ::CDRQ(bool testModeEnabled) {
	isInTestMode = testModeEnabled;
	queue = new cPacketQueue();
	currSendHeadCDRQPacket = new SendHeadAndDeletePacket(kSTRING_CDRQ_SENDHEAD);
	logwriter = new LogFileWriter();
}

/**
 * default destructor
 */
CDRQ::~CDRQ() {
	delete queue;
	delete currentBbdelay;
	delete logwriter;
	cancelAndDelete(currSendHeadCDRQPacket);
}

/**
 * default initializer
 */
void CDRQ::initialize() {
	currentBbdelay = new Bbdelay(0.005, true, "DUMMY");

	if (!isInTestMode) {
		signalBbdelay = registerSignal(kSIGNAL_CDRQ_BBDELAY);
		emit(signalBbdelay, currentBbdelay->bbdelay());
	}
}

/**
 * Will log some information in logfiles. Kind of information depens on given filename
 * @param job for logging
 * @param filename for saving
 */
void CDRQ::writeLogToHarddrive(MyPacket* job, std::string filename, simtime_t currentDelay) {
	// check for current localtime, currently not needed
	if (localTime.empty()) {
		Localtime *lt = new Localtime();
		localTime = lt->currentLocaltimeForLogfile(false).c_str();
		delete (lt);
	}
	localTime = "";

	// make folder
	const char *folder = par(kPAR_LOGFILEPREFIX).stringValue();
	mkdir(folder, S_IRWXU);

	// build path
	outputPath[0] = 0;
	char buff[50];
	sprintf(buff, "%s%s%s.txt", folder, localTime.c_str(), filename.c_str());
	outputPath = buff;

	// choose functions...depends on given filename
	if (filename.compare(kSTRING_FILE_DOWNLOADBBDELAY) == 0) {
		logwriter->writeLogfileForBbdelay(outputPath, currentBbdelay);
	} else if (filename.compare(kSTRING_FILE_CDRQ_SEND) == 0) {
		logwriter->writeLogfileForPacketsSendQueue(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_CDRQ_RECEIVED) == 0) {
		logwriter->writeLogfileForPacketsReceivedQueue(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_DOWNLOADPACKETSBBDELAY) == 0) {
		logwriter->writeLogFileForPacketsBbdelay(outputPath, job, currentBbdelay, currentDelay);
	} else if (filename.compare(kSTRING_FILE_CDRQ_SENDHEAD) == 0) {
		logwriter->writeLogFileForPacketsSendHead(outputPath, currSendHeadCDRQPacket);
	}
}

/**
 * handling incoming messages in the simple module
 * we will look for the kind of message (self message, unknown, ...) and act like it has to be
 *
 * @param msg, incoming cMessage*
 */
void CDRQ::handleMessage(cMessage* msg) {
	SendHeadAndDeletePacket* sendHead = dynamic_cast<SendHeadAndDeletePacket*>(msg);
	MyPacket* job = dynamic_cast<MyPacket*>(msg);

	// check for control message
	if (strcmp(msg->getFullName(), kSTRING_CONTROL_CDRQ) == 0) {
		handleControlMessage(msg);

		// check for normal packet
	} else if (job != 0) {
		// add packet to queue and log it
		addPacketToQueue(job, simTime());
		if (!isInTestMode) {
			if (par(kPAR_WRITINGLOGFILEQUEUE).boolValue()) {
				writeLogToHarddrive(job, kSTRING_FILE_CDRQ_RECEIVED, SIMTIME_ZERO);
			}
		}
		// check for send head event
	} else if (sendHead != 0) {
		// dispatch head events, schedule new self msgs
		EV<< getFullPath() << " handles SendHeadAndDeletePacket for " << sendHead->getDeletePacketWithName() << endl;
		dispatch(simTime());

		// all checks failed, trop the packet
	} else if (!isInTestMode) {
		EV << getFullPath() << " unknown message: " << msg->getFullName()
		<< ", type (" << msg << "), at " << simTime()<< endl;
		delete(job);
		delete(msg);
	}
}

		/**
		 * Handles control packet for changing delays
		 * @param ControlPacket*
		 */
void CDRQ::handleControlMessage(cMessage* msg) {
	EV<<getFullPath()<<" handles Packet for bbDelayChanged at " << msg->getArrivalTime() << endl;

	// get new delays and call bbDelayChanged
	cMsgPar *par = reinterpret_cast<cMsgPar*>(msg->getParList()[msg->findPar(kSTRING_NEWBBDELAYPOINTER)]);
	Bbdelay* newBbdelay = reinterpret_cast<Bbdelay*>(par->pointerValue());
	bbDelayChanged(newBbdelay, simTime());

	delete(msg);
	delete(newBbdelay);
}

	/**
	 * add a packet to the queue
	 *
	 * @param packet to add
	 * @param currentSimtime simulation time
	 */
void CDRQ::addPacketToQueue(MyPacket* job, simtime_t currentSimtime) {
	bool isQueueEmpty = queue->isEmpty();

	// add timestamp
	job->setTimestampsArraySize((1 + job->getTimestampsArraySize()));
	char timelog[50];
	sprintf(timelog, "%s %s,%.0f", kSTRING_CDRQ, kSTRING_RECEIVED, currentSimtime.dbl() * kMULTIPLIER_S_TO_NS);
	job->setTimestamps((job->getTimestampsArraySize() - 1), timelog);
	job->setDCDRQ(currentSimtime);

	// manipulate tstamp, when packet is dropped
	if (job->getDroppedSimulated()) {
		job->setArrivalTimeForLogging(job->getSendingTimeForLogging());
	} else {
		job->setArrivalTimeForLogging(currentSimtime);
	}

	// log packet for bbdelay
	if (job->getPacketNumber() == 0) {
		currentBbdelay->setCMsgNumberOfProbePackets(job->getNumberOfProbePackets());
		currentBbdelay->setCMsgPayloadSize(job->getPayloadSize());
		currentBbdelay->setDelay(currentSimtime - job->getStartTimeForLogging());
		currentBbdelay->setDroprate(job->getDroprateQueue());
	}

	// add time for the earliest delivery
	if (bbDelayVector.size() > 0 && bbTimeVector.size() > 0) {
		simtime_t delay = calculateBackboneDelay(currentSimtime);
		job->setEarliestTimeForDelivery(currentSimtime + delay);
	}

	// insert packet into queue
	if (isInTestMode)
		queue->insert(job->dup());
	else
		queue->insert(job);

	// starting self messaging process when queue is empty
	if (isQueueEmpty) {
		//update the vectors
		bbDelayVector.push_back(currentBbdelay);
		bbTimeVector.push_back(currentSimtime);

		// schedule self message
		scheduleNewSendHeadAndDeletePacket(currentSimtime);
	}
}

/**
 * Will schedule a new self-message (which will send head of queue and pop it)
 * @param currentSimtime: actually simulation time
 */
void CDRQ::scheduleNewSendHeadAndDeletePacket(simtime_t currentSimtime) {
	if (currSendHeadCDRQPacket->isScheduled()) {
		EV<< getFullPath() << " currSendHeadCDSQPacket is currently scheduled! SHOULD NOT HAPPEN";
	}

	simtime_t delay;
	// get earliest time for delivery or calculate the time
	if (((MyPacket*)queue->front())->getEarliestTimeForDelivery() == SIMTIME_ZERO) {
		delay = calculateBackboneDelay(currentSimtime);
		// sub the time, which is already gone
		simtime_t waited = currentSimtime - ((MyPacket*)queue->front())->getArrivalTime();
		delay -= delay < SIMTIME_ZERO ? delay : waited;
		((MyPacket*)queue->front())->setEarliestTimeForDelivery(currentSimtime + delay);
	} else {
		delay = ((MyPacket*)queue->front())->getEarliestTimeForDelivery() - currentSimtime;
	}

	// dropped package got no delay
	if (((MyPacket*)queue->front())->getDroppedSimulated()) {
		delay = SIMTIME_ZERO;
	}

	// set new simtimes
	currSendHeadCDRQPacket->setDeletePacketWithName(((MyPacket*)queue->front())->getName());
	currSendHeadCDRQPacket->setSendingTimeForTesting(currentSimtime);
	currSendHeadCDRQPacket->setArrivalTimeForTesting(currentSimtime+delay);

	// output in tkenv
	if (!isInTestMode) {
		// set datarate for logging
		Bbdelay tmp = *currentBbdelay;
		((MyPacket*) queue->front())->setBbdelay(tmp);
		// send self msg
		scheduleAt(currentSimtime + delay, currSendHeadCDRQPacket);
		EV<< getFullPath() << " scheduled SendHeadAndDeletePacket with d_bb: " << delay << endl;
		// logging
		if (par(kPAR_WRITINGLOGFILEQUEUE)) {
			writeLogToHarddrive((MyPacket*)queue->front(), kSTRING_FILE_DOWNLOADPACKETSBBDELAY, delay);
			writeLogToHarddrive((MyPacket*)queue->front(), kSTRING_FILE_CDRQ_SENDHEAD, SIMTIME_ZERO);
		}
	}
}

		/**
		 * Will schedule a new self-message (which will send head of queue and pop it) with given delay
		 * @param currentSimtime: actually simulation time
		 * @param delay: delay, when the self-message will be started
		 */
void CDRQ::scheduleNewSendHeadAndDeletePacket(simtime_t currentSimtime, simtime_t delay) {
	if (currSendHeadCDRQPacket->isScheduled()) {
		EV<< getFullPath() << " currSendHeadCDSQPacket is currently scheduled! SHOULD NOT HAPPEN";
	}
	// dropped package got no delay
	if (((MyPacket*)queue->front())->getDroppedSimulated()) {
		delay = SIMTIME_ZERO;
	}

	// set new simtimes
	currSendHeadCDRQPacket->setDeletePacketWithName(((MyPacket*)queue->front())->getName());
	currSendHeadCDRQPacket->setSendingTimeForTesting(currentSimtime);
	currSendHeadCDRQPacket->setArrivalTimeForTesting(currentSimtime+delay);

	// vorher könnten bbdelay usw. 0 sein
	((MyPacket*)queue->front())->setEarliestTimeForDelivery(currentSimtime + delay);

	// output in tkenv
	if (!isInTestMode) {
		// set datarate for logging
		Bbdelay tmp = *currentBbdelay;
		((MyPacket*) queue->front())->setBbdelay(tmp);
		// send self msg
		scheduleAt(currentSimtime + delay, currSendHeadCDRQPacket);
		EV<< getFullPath() << " scheduled SendHeadAndDeletePacket with d_bb: " << delay << endl;
		// logging
		if (par(kPAR_WRITINGLOGFILEQUEUE)) {
			writeLogToHarddrive((MyPacket*)queue->front(), kSTRING_FILE_DOWNLOADPACKETSBBDELAY, delay);
			writeLogToHarddrive((MyPacket*)queue->front(), kSTRING_FILE_CDRQ_SENDHEAD, 0);
		}
	}
}

		/**
		 * react on changes of the underlying network (due to client movement)
		 * @param newDatarate
		 * @param currentSimtime
		 */
void CDRQ::bbDelayChanged(Bbdelay* newBbdelay, simtime_t currentSimtime) {
	//check if valid
	//if the new backbone is valid and changed in regards to the last we need to make changes
	bool delayChanged = false;

	if (newBbdelay->validbbdelay() && newBbdelay->bbdelay() != currentBbdelay->bbdelay()) {
		//save the newBbdelay
		currentBbdelay = newBbdelay->copy();
		delayChanged = true;

		//if queue is not empty we need to recalculate capacity and change the backbone vector
		if (!queue->empty()) {

			//add the current values to the vectors
			bbDelayVector.push_back(currentBbdelay);
			bbTimeVector.push_back(currentSimtime);
		}
	}

//	if (queue->isEmpty() && delayChanged) {
//		currentBbdelay->setBbdelaySim(newBbdelay->bbdelay());
//	}

	// output in tkenv and manipulating self messages, cause we got new delays
	if (!isInTestMode) {
		if (delayChanged) {
			char c[50];
			currentBbdelay->toString(c);
			EV<< getFullPath() << " backbone delay changed to " << c << " at " << currentSimtime <<endl;

			manipulateSelfMessageProcess(currentSimtime);
			emit(signalBbdelay, currentBbdelay->bbdelay());
		}

		writeLogToHarddrive(NULL, kSTRING_FILE_DOWNLOADBBDELAY, SIMTIME_ZERO);

	}
}

			/**
			 * cancels current self message, which simulates backbone delay, when scheduled and backbone was changed.
			 * even schedules new self message or calls dispatch directly
			 */
void CDRQ::manipulateSelfMessageProcess(simtime_t currentSimtime) {
	if ((currSendHeadCDRQPacket->isScheduled() && !queue->isEmpty()) || isInTestMode) {
		cancelEvent(currSendHeadCDRQPacket);

		// get delay
		simtime_t delay = calculateRestBackboneDelay((MyPacket*) queue->front(), currentSimtime);

		// is delay valid?
		if (delay < 0) {
			delay = SIMTIME_ZERO;
		}

		// set new earliest time for delivery
		((MyPacket*) queue->front())->setEarliestTimeForDelivery(currentSimtime + delay);

		// we waited longer than needed OR there is some time to wait
		scheduleNewSendHeadAndDeletePacket(currentSimtime, delay);

		for (int i = 0; i < queue->length(); i++) {
			simtime_t newDelay = calculateRestBackboneDelay((MyPacket*) queue->get(i), currentSimtime);
			((MyPacket*) queue->get(i))->setEarliestTimeForDelivery(currentSimtime + newDelay);
		}
	}
}

/**
 * calculates and return current backbone delay in addiction to the past delays/times
 * @param currentSimTime
 */
simtime_t CDRQ::calculateBackboneDelay(simtime_t currentSimtime) {
	simtime_t runtime = currentSimtime - bbTimeVector.front();
	simtime_t bbDelay = SIMTIME_ZERO;
	simtime_t endtime = currentSimtime;
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
 * @param currentSimTime simulation time
 */
simtime_t CDRQ::calculateRestBackboneDelay(MyPacket* job, simtime_t currentSimtime) {
	// we waited since MyPacket arrived
	simtime_t delayGone = currentSimtime - job->getArrivalTimeForLogging();
	// but, now we have to wait....
	simtime_t delayNew = calculateBackboneDelay(currentSimtime);
	// so there is just a little delay at the end
	simtime_t delayWait = delayNew - delayGone;
	return delayWait < SIMTIME_ZERO ? SIMTIME_ZERO : delayWait;
}

/**
 * this function handles the transmitting of the head packet and the loss of
 * packets The function first checks if there is enough capacity to send the
 * packet
 */
void CDRQ::dispatch(simtime_t currentSimtime) {
	// send head, if queue is not empty
	if (!queue->isEmpty()) {
		MyPacket* head = (MyPacket*) queue->pop();

		// add timestamp
		head->setTimestampsArraySize((1 + head->getTimestampsArraySize()));
		char timelog[50];
		sprintf(timelog, "%s %s %f", kSTRING_CDRQ, kSTRING_SEND, currentSimtime.dbl());
		head->setTimestamps((head->getTimestampsArraySize() - 1), timelog);
		head->setSCDRQ(currentSimtime);

		// reset earliest time for delivery
		head->setEarliestTimeForDelivery(SIMTIME_ZERO);

		// add logging send time
		if (!head->getDroppedSimulated())
			head->setSendingTimeForLogging(currentSimtime);

		// write log and send packet only, when we are not testing..otherwise drop it
		if (!isInTestMode) {
			if (par(kPAR_WRITINGLOGFILEQUEUE).boolValue()) {
				writeLogToHarddrive(head, kSTRING_FILE_CDRQ_SEND, SIMTIME_ZERO);
			}

			EV<< getFullPath() << " sends " << head << endl;
			sendDelayed(head, SIMTIME_ZERO, "out");
		} else {
			delete head;
		}

		// self messaging process
		if (!queue->isEmpty()) {
			if (((MyPacket*) queue->front())->getEarliestTimeForDelivery() < currentSimtime) {
				dispatch(currentSimtime);
			} else {
				scheduleNewSendHeadAndDeletePacket(currentSimtime);
			}
		}
	}

	if (queue->isEmpty()) {
		//as the queue is empty now we clear all stored accumulation
		bbDelayVector.clear();
		bbTimeVector.clear();
	} else {
		//the queue is not empty so we need to change some values
		bbDelayVector.clear();
		bbTimeVector.clear();
		bbDelayVector.push_back(currentBbdelay);
		bbTimeVector.push_back(currentSimtime);
	}
}
}
;

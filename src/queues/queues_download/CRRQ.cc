/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "CRRQ.h"

namespace projekt {

// this class defines the simple module with name CRRQ
Define_Module(CRRQ)
;

/**
 * default constructor; set test mode to false and init variabeles
 */
CRRQ::CRRQ() {
	isInTestMode = false;
	initialize();
	queue = new cPacketQueue();
	currSendHeadCRRQPacket = new SendHeadAndDeletePacket(kSTRING_CRRQ_SENDHEAD);
	logwriter = new LogFileWriter();
}

/**
 * default constructor; set test mode and init variabeles
 * @param testModeEnabled, true in test modes
 */
CRRQ::CRRQ(bool testModeEnabled) {
	isInTestMode = testModeEnabled;
	initialize();
	queue = new cPacketQueue();
	currSendHeadCRRQPacket = new SendHeadAndDeletePacket(kSTRING_CRRQ_SENDHEAD);
	logwriter = new LogFileWriter();
}

/**
 * default destructor; garbage collection
 */
CRRQ::~CRRQ() {
	delete queue;
	delete currentDatarate;
	delete logwriter;
	cancelAndDelete(currSendHeadCRRQPacket);
}

/**
 * default initializer; init default loss prob and datarate
 * if not in test mode, we will init signals too
 */
void CRRQ::initialize() {
	freeBufferInByte = INT32_MAX;

	//we all dont like nullpointers
	//some initialization in Bytes value to overcome null pointer exceptions that happen at first dataratechange event
	currentLossProbability = 0.0;
	currentDatarate = new Datarate(1000, true, 0.0, "DUMMY");
	currentDatarate->setDatarateSim(1000);

	if (!isInTestMode) {
		//signals for collecting data (statistics)
		signalDatarate				= registerSignal(kSIGNAL_CRRQ_DATARATE);
		signalLossprob				= registerSignal(kSIGNAL_CRRQ_LOSSPROB);
		signalLossprobRnd			= registerSignal(kSIGNAL_CRRQ_LOSSPROB_RND);
		signalPacketDatarateDelay 	= registerSignal(kSIGNAL_CRRQ_PACKET_DATARATE_DELAY);
		signalDropMeasured			= registerSignal(kSIGNAL_CRRQ_DROPMEASURED);
		signalDropSimulated			= registerSignal(kSIGNAL_CRRQ_DROPSIMULATED);

		emit(signalDatarate, currentDatarate->datarateSim());
		emit(signalLossprob, currentLossProbability);
	}
}

/**
 * Will log some information in logfiles. Kind of information depens on given filename
 * @param job for logging
 * @param filename for saving
 * @param currentDelay optional
 */
void CRRQ::writeLogToHarddrive(MyPacket* job, std::string filename, simtime_t currentDelay) {
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
	if (filename.compare(kSTRING_FILE_DOWNLOADDATARATE) == 0) {
		logwriter->writeLogfileForDatarate(outputPath, currentDatarate);
	} else if (filename.compare(kSTRING_FILE_DOWNLOADCHAR) == 0) {
		logwriter->writeLogfileForPacketsChar(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_CRRQ_RECEIVED) == 0) {
		logwriter->writeLogfileForPacketsReceivedQueue(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_CRRQ_SEND) == 0) {
		logwriter->writeLogfileForPacketsSendQueue(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_DOWNLOADPACKETSDATARATE) == 0) {
		logwriter->writeLogFileForPacketsDatarate(outputPath, job, currentDatarate, currentDelay);
	} else if (filename.compare(kSTRING_FILE_CRRQ_SENDHEAD) == 0) {
		logwriter->writeLogFileForPacketsSendHead(outputPath, currSendHeadCRRQPacket);
	}
}

/**
 * handling incoming messages in the simple module
 * we will look for the kind of message (self message, unknown, ...) and act like it has to be
 * @params msg, cMessage*
 */
void CRRQ::handleMessage(cMessage* msg) {
	SendHeadAndDeletePacket* sendHead = dynamic_cast<SendHeadAndDeletePacket*>(msg);
	MyPacket* job = dynamic_cast<MyPacket*>(msg);

	// is message a control message?
	if (strcmp(msg->getFullName(), kSTRING_CONTROL_CRRQ) == 0) {
		handleControlMessage(msg);

	// or send head event?
	} else if (sendHead != 0) {
		handleSendHeadAndDeletePacket(simTime(), sendHead);

	// or a job ?
	} else if (job != 0) {
		addPacketToQueue(job, simTime());
		if (!isInTestMode) if (par(kPAR_WRITINGLOGFILEQUEUE).boolValue()) {
			writeLogToHarddrive(job, kSTRING_FILE_CRRQ_RECEIVED, 0.0);
		}

	// no..drop it
	} else if (!isInTestMode) {
		EV<< getFullPath() << " unknown message: " << msg->getFullName()
		<< ", type (" << msg << "), at " << simTime()<< endl;
	}
}

/**
 * Handles control packet for changing delays
 * @param msg*
 */
void CRRQ::handleControlMessage(cMessage* msg) {
	EV<<getFullPath()<<" handles Packet for datarateChanged at " << msg->getArrivalTime() << endl;

	// getting new Datarate*
	cMsgPar *par = reinterpret_cast<cMsgPar*>(msg->getParList()[msg->findPar(kSTRING_NEWDATARATEPOINTER)]);
	Datarate* newDatarate = reinterpret_cast<Datarate*>(par->pointerValue());
//		char* c = new char[50];
//		newDatarate->toString(c);
//		EV <<"\thandleControlMessage: " << c <<endl;
	datarateChanged(newDatarate, simTime());

	// garbage collection
	delete(msg);
	delete(newDatarate);
}

/**
 * Handles SendHeadAndDeletePacket. Will calculate bbdelay for current head of queue,
 * call dispatch for the current head and schedule new SendHeadAndDeletePacket
 * @param currentSimtime
 * @param sendHead
 */
void CRRQ::handleSendHeadAndDeletePacket(simtime_t currentSimtime, SendHeadAndDeletePacket* sendHead) {
	// calculate d_bb and send packet to CDSQ
	EV<< getFullPath() << " handles SendHeadAndDeletePacket for " << sendHead->getDeletePacketWithName() << endl;

	// dispatch only, when quee is not empty
	if (!queue->isEmpty()) {
		dispatch(currentSimtime, NULL);

		scheduleNewSendHeadAndDeletePacket(currentSimtime);
	} else {
		EV<< getFullPath() << " gets 'SendHeadAndDelete', but queue is empty, " << " at " << currentSimtime << endl;
	}
}

/**
 * Will schedule a new self-message (which will send head of queue and pop it).
 * @param currentSimtime
 */
void CRRQ::scheduleNewSendHeadAndDeletePacket(simtime_t currentSimtime) {
	if (!queue->empty()) {
		simtime_t d_air = calculateDatarateDelay(((MyPacket*) queue->front())->getPayloadSize(), currentSimtime);
		// dropped packages got no delay
		if (((MyPacket*) queue->front())->getDroppedSimulated()) {
			d_air = 0.0;
		}

		// setting new simtimes
		currSendHeadCRRQPacket->setDeletePacketWithName(((MyPacket*) queue->front())->getName());
		currSendHeadCRRQPacket->setArrivalTimeForTesting(currentSimtime + d_air);
		currSendHeadCRRQPacket->setSendingTimeForTesting(currentSimtime);

		// schedule new self message and log stuff only, when not in test mode
		if (!isInTestMode) {
			// TODO
			// addind a micro second for untested logfiles
//			while ((currentSimtime + d_air) < simTime()){
//				d_air += 1/1000 / kMULTIPLIER_S_TO_NS;
//			}
			// send self msg
			scheduleAt(currentSimtime + d_air, currSendHeadCRRQPacket);
			EV<< getFullPath() << " scheduled SendHeadAndDeletePacket with d_air: " << d_air << endl;
			// logging
			if (par(kPAR_WRITINGLOGFILEQUEUE)) {
				writeLogToHarddrive((MyPacket*) queue->front(), kSTRING_FILE_DOWNLOADPACKETSDATARATE, d_air);
				writeLogToHarddrive((MyPacket*) queue->front(), kSTRING_FILE_CRRQ_SENDHEAD, 0);
			}
		}
	}
}

/**
 * Will schedule a new self-message (which will send head of queue and pop it) with given delay
 * @param currentSimtime: actually simulation time
 * @param delay: delay, when the self-message will be started
 */
void CRRQ::scheduleNewSendHeadAndDeletePacket(simtime_t currentSimtime, simtime_t delay) {
	if (!queue->empty()) {

		// set new simtimes
		currSendHeadCRRQPacket->setDeletePacketWithName(((MyPacket*) queue->front())->getName());
		currSendHeadCRRQPacket->setArrivalTimeForTesting(currentSimtime + delay);
		currSendHeadCRRQPacket->setSendingTimeForTesting(currentSimtime);

		// schedule new self message and log stuff only, when not in test mode
		if (!isInTestMode) {
			// send self msg
			scheduleAt(currentSimtime + delay, currSendHeadCRRQPacket);
			EV<< getFullPath() << " scheduled SendHeadAndDeletePacket with d_air: " << delay << endl;
			// logging
			if (par(kPAR_WRITINGLOGFILEQUEUE)) {
				writeLogToHarddrive((MyPacket*) queue->front(), kSTRING_FILE_DOWNLOADPACKETSDATARATE, delay);
				writeLogToHarddrive((MyPacket*) queue->front(), kSTRING_FILE_CRRQ_SENDHEAD, 0);
			}
		}
	}
}

/**
 * add packet to the queue
 * @param job
 * @param currentSimtime
 */
void CRRQ::addPacketToQueue(MyPacket* job, simtime_t currentSimtime) {
	// add timestamp
	job->setTimestampsArraySize((1 + job->getTimestampsArraySize()));
	char timelog[50];
	sprintf(timelog, "%s %s,%.0f", kSTRING_CRRQ, kSTRING_RECEIVED, currentSimtime.dbl() * kMULTIPLIER_S_TO_NS);
	job->setTimestamps((job->getTimestampsArraySize() - 1), timelog);
	job->setDCRRQ(currentSimtime);

	if (job->getDroppedSimulated()) {
		job->setArrivalTimeForLogging(job->getSendingTimeForLogging());
	} else {
		job->setArrivalTimeForLogging(currentSimtime);
	}

	// log packet for datarate
	if (job->getPacketNumber() == 0){
		currentDatarate->setCMsgNumberOfProbePackets(job->getNumberOfProbePackets());
		currentDatarate->setCMsgPayloadSize(job->getPayloadSize());
	}

	//if queue is empty, store some data
	bool isQueueEmpty = queue->empty();
	if (isQueueEmpty) {
		//update the vectors
		datarateVector.push_back(currentDatarate);
		drTimeVector.push_back(currentSimtime);

		//set capacity to zero, because we had no transmitted data yet
		capacityBytes = 0;
	}

	bool jobInserted = false;
	// Add the packet to the queue if there is space left in the queue - if not, drop it.
	if (freeBufferInByte - job->getPayloadSize() > 0) {
		freeBufferInByte -= job->getPayloadSize();
		if (isInTestMode) queue->insert(job->dup());
		else queue->insert(job);
		jobInserted = true;
	} else {
		EV<< getFullPath() << " dropped " << job << " (no free buffer)" << " at " << currentSimtime << endl;
		delete(job);
	}

		// if queue was empty, we will start the self messaging process, that will send the heads
	if (isQueueEmpty && jobInserted) {
		scheduleNewSendHeadAndDeletePacket(currentSimtime);
	}
}

/**
 * calculate the loss probability for this packet on basis of time[] and perf[]
 * @param currentSimtime - the time the current head packet will be
 * transmitted or lost
 * @return droprate as double
 */
double CRRQ::calculateLossProbability(simtime_t currentSimtime) {
	//check if we really transmit the packet or if it is lost
	//calculate the median loss probability
	simtime_t runtime = (currentSimtime - drTimeVector.front()).dbl();
	double loss = 0.0;
	simtime_t endtime = currentSimtime.dbl();
	simtime_t starttime;
	for (int i = datarateVector.size() - 1; i >= 0; i--) {
		starttime = drTimeVector.at(i).dbl();
		loss += (datarateVector.at(i)->droprate() * ((endtime - starttime) / runtime));
		endtime = starttime;
	}
	currentLossProbability = loss;

//	std::cout << "crrq loss prob at " << currentSimtime << ":\t" << loss << " (" << datarateVector.size() << ")\n";

	return loss;
}

/**
 * Calculates the transmission delay d_air
 */
simtime_t CRRQ::calculateDatarateDelay(double packetsize, simtime_t currentSimtime) {
	simtime_t runtime = currentSimtime - drTimeVector.front();
	simtime_t datarateDelay = 0.0;
	simtime_t endtime = currentSimtime;
	simtime_t starttime;
//	long double datarate, endt, startt, runt, delay, res;

	// if runtime equals 0, we return the currentDatarate
	if (runtime== SIMTIME_ZERO){
		return currentDatarate->datarateSim() == 0 ? kDEFAULTDELAY : packetsize / currentDatarate->datarateSim();
	}

	// adding all delays
	for (int i = datarateVector.size() - 1; i >= 0; i--) {
		starttime = drTimeVector.at(i).dbl();
//		datarate = datarateVector.at(i)->datarateSim();
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
 * @param currentSimtime
 */
void CRRQ::datarateChanged(Datarate* newDatarate, simtime_t currentSimtime) {
	bool datarateChanged = newDatarate->validdatarate();

	if (newDatarate->validdatarate() && (newDatarate->datarateSim() != currentDatarate->datarateSim())) {
		currentDatarate = newDatarate->copy();
		datarateChanged = true;
	} else if (newDatarate->modemMode().compare(currentDatarate->modemMode()) != 0
			|| newDatarate->droprate() != currentDatarate->droprate()) {
		//only modemmode or droprate changed - combine the data
		currentDatarate->setDroprate(newDatarate->droprate());
		currentDatarate->setModemMode(newDatarate->modemMode());
//			new Datarate(currentDatarate->datarate(), currentDatarate->validdatarate(),
//				newDatarate->droprate(), newDatarate->modemMode());
		datarateChanged = true;
	}

	// set simulated datarate
//	if (datarateChanged){
//		currentDatarate->setDatarateSim(newDatarate->datarate());
//	}

//	std::cout << "\t\t\tcrrq datarate change event: " << (datarateChanged? "true " : "false")
//			<< ", valid: " << (newDatarate->validdatarate() ? "true " : "false") << "\n";


	if (!queue->empty() && datarateChanged) {
		//calculate the number of bytes that went over the air since the last
		//transmitted packet or networkchange
		if (drTimeVector.size() == 0 || datarateVector.size() == 0) {
			capacityBytes = 0;
		} else {
			capacityBytes += (currentSimtime - drTimeVector.back()).dbl() * datarateVector.back()->datarateSim();
		}

		if (capacityBytes < 0) {
			EV<< getFullPath() << " capacity < 0  " << capacityBytes << " at " << currentSimtime << endl;
		}

			//add the current values to the vectors
		datarateVector.push_back(currentDatarate);
		drTimeVector.push_back(currentSimtime);
		if (!isInTestMode) {
			char c[50];
			currentDatarate->toString(c);
			EV<< getFullPath() << " datarate changed to " << c << " at " << currentSimtime <<endl;

			// if testmode is disabled, log to harddrive and manipulate self message process
			// because we have no delays
			manipulateSelfMessageProcess(currentSimtime);
			emit(signalDatarate, currentDatarate->datarateSim());
			emit(signalLossprob, calculateLossProbability(currentSimtime));
		} else {
			manipulateSelfMessageProcess(currentSimtime);
			emit(signalDatarate, currentDatarate->datarateSim());
			emit(signalLossprob, -1);
		}
	}

	// if testmode is disabled, log to harddrive and manipulate self message process
	// because we have no delays
	if (!isInTestMode && datarateChanged) {
		writeLogToHarddrive(NULL, kSTRING_FILE_DOWNLOADDATARATE, 0.0);
	}
}

/**
 * cancels current self message, which simulates datarate delay, when scheduled and datarate was changed.
 * even schedules new self message or calls dispatch directly
 * @param currentSimtime
 */
void CRRQ::manipulateSelfMessageProcess(simtime_t currentSimtime) {
	// catch errors
	if (currentDatarate->datarateSim() == 0){
		return;
	}

	if ((currSendHeadCRRQPacket->isScheduled() && !queue->isEmpty()) || isInTestMode) {
		cancelEvent(currSendHeadCRRQPacket);

		// calculate the number of bytes that went over the air since the last transmitted packet or networkchange
		double bytesLeft = ((MyPacket*) queue->front())->getPayloadSize() - capacityBytes;

		// calculate delay for the left bytes
		simtime_t delayToWait = bytesLeft / currentDatarate->datarateSim();

		// we waited longer than needed OR there is some time to wait
		if (delayToWait < 0) {
			dispatch(currentSimtime, NULL);
			currSendHeadCRRQPacket->setSendingTimeForTesting(currentSimtime);
			currSendHeadCRRQPacket->setArrivalTimeForTesting(currentSimtime);
		} else {
			scheduleNewSendHeadAndDeletePacket(currentSimtime, delayToWait);
		}
	}
}

/**
 * this function handles the transmitting of the head packet and the loss of
 * packets The function first checks if there is enough capacity to send the packet
 * @param currentSimtime
 * @param retBool, [0] = everything executed normally, [1] = msgLost
 */
void CRRQ::dispatch(simtime_t currentSimtime, bool retBool[]) {
	double newcapacity;

	//if the called method doesnt need return booleans, we will create dummys
	if (retBool == NULL) {
		retBool = new bool[2];
	}

	if (!queue->empty()) {
		//calculate the number of bytes that went over the air since the last transmitted packet or networkchange
		double add = (currentSimtime - drTimeVector.back()).dbl() * datarateVector.back()->datarateSim();
		newcapacity = capacityBytes + add;

		//only transmit if we really have enough capacity for the packet
		// WE HAVE ENOUGH CAPACITY, because sendHeadAnd... had latency, so we got capacity :)
		double diff = newcapacity - ((MyPacket*) queue->front())->getPayloadSize();

		if (diff >= 0 || fabs(diff) < kEPS3) {
			//don't substract the heads.packetsize here, this will be done in transmitorloosehead
			capacityBytes = newcapacity;

			if (capacityBytes < DBL_MIN && !isInTestMode) {
				EV<< getFullPath() << " capacity < 0 in handleMessage " << capacityBytes << " at " << currentSimtime << endl;
			}
				//transmitOrLoose the head packet
			transmitOrLooseHead(currentSimtime, retBool);

			if (retBool[0]) {
				//now we should have 0.0 <= capacityBytes <1.0
				if (capacityBytes > 1.0 && !isInTestMode) {
					EV<< getFullPath() << " capacity > 1 after handleMessage  " << capacityBytes << " at " << currentSimtime << endl;
				}
			}
		} else {
			if (!isInTestMode)
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
 * @param currentSimtime
 * @param retBool, [0] = transmitOrLooseHead executed normally, [1] = msgLost
 */
void CRRQ::transmitOrLooseHead(simtime_t currentSimtime, bool retBool[]) {
	bool isQueueEmpty = queue->empty();
	bool msgLost = false;
	MyPacket* head = (MyPacket*) queue->pop();

	// add logging send time
	if (!head->getDroppedSimulated()) head->setSendingTimeForLogging(currentSimtime);

	double diff = capacityBytes - head->getPayloadSize();
	double loss = calculateLossProbability(currentSimtime);
	head->setSCRRQ(currentSimtime);
	head->setDroprateQueue(loss);
	head->setDatarate(*currentDatarate);

	if (!isInTestMode){
		if (par(kPAR_WRITINGLOGFILEQUEUE).boolValue()) {
			writeLogToHarddrive(head, kSTRING_FILE_CRRQ_SEND, SIMTIME_ZERO);
		} else if (par(kPAR_WRITINGLOGFILENODE).boolValue()){
			writeLogToHarddrive(head, kSTRING_FILE_DOWNLOADCHAR, SIMTIME_ZERO);
		}
	}
	if (!isQueueEmpty && (diff >= 0 || fabs(diff) < kEPS3)) {
		//subsctract the used capacity from the available capacity
		//ignore rounding errors smaller than eps
		capacityBytes = (diff >= 0) ? diff : 0;

		//add free space to queue
		freeBufferInByte += head->getPayloadSize();

		//check if we really transmit the packet or if it is lost
		double random = dblrand();
		if (!isInTestMode){
			emit(signalLossprobRnd, random);
		}

		// add timestamp
		head->setTimestampsArraySize((1 + head->getTimestampsArraySize()));
		char timelog[50];
		sprintf(timelog, "%s %s %f", kSTRING_CRRQ, kSTRING_SEND, currentSimtime.dbl());
		head->setTimestamps((head->getTimestampsArraySize() - 1), timelog);

		//emitting signals
		if (!isInTestMode){
			emit(signalDropMeasured,(random<loss) ? 1 : 0);
			emit(signalDropSimulated,head->getDroppedMeasured() ? -1 : 0);
		}

		if (random >= loss) {
			msgLost = false;

			if (!isInTestMode) {
				EV<< getFullPath() << " send " << head << " at " << currentSimtime << endl;
				// sendDelayed cannot be called in test files (wrong module)
				sendDelayed(head, SIMTIME_ZERO, "out");
			} else {
				delete (head);
			}
		} else {

			head->setDroppedSimulated(true);
			if (!isInTestMode) {
				EV << getFullPath() << " lost " << head << " at " << currentSimtime << endl;
				sendDelayed(head, SIMTIME_ZERO, "out");
			} else {
				delete(head);
			}

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
			drTimeVector.push_back(currentSimtime);
		}
		retBool[0] = true;
	} else {
		retBool[0] = false;
	}
	retBool[1] = msgLost;
}

}
;

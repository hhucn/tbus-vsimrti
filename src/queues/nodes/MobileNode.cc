/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "MobileNode.h"

namespace projekt {

Define_Module(MobileNode)
;

/**
 * default constructor
 */
MobileNode::MobileNode() {
	isInTestMode = false;
}

/**
 * custom constructor for defining testMode
 */
MobileNode::MobileNode(bool testMode) {
	isInTestMode = testMode;
}

/**
 * default destructor, delete and cancel all references / messages
 */
MobileNode::~MobileNode() {
	cancelAndDelete(selfMsgChangeDelays);
	cancelAndDelete(selfMsgStartTrain);
	delete logwriter;
	delete firstInTrainNonDroppedMeasuredPacket;
	delete firstInTrainNonDroppedSimulatedPacket;
	delete lastInTrainNonDroppedMeasuredPacket;
	delete lastInTrainNonDroppedSimulatedPacket;
}

/**
 * default initializer
 */
void MobileNode::initialize() {
	logwriter = new LogFileWriter();

	// createing self messages
	selfMsgChangeDelays = new cMessage(kPAR_CHANGEPACKETSDELAY);
	selfMsgStartTrain = new cMessage(kSTRING_STARTTRAIN);

	// some initial values, a msg box, messages and output path
	sendInitialMessage();
	showInitialMsgBox();

	// init boolean
	firstNonDroppedMeasuredPacket = false;
	firstNonDroppedSimulatedPacket = false;

	// init counter
	controlPacketsSequenceNumber = 0;
	sequenceNumber = 0;
	sequenceNumber = -1;
	countMsgSend = 0;
	countPacketSend = 0;
	countDownloadReceivedReal = 0;
	countDownloadReceivedSim = 0;
	countDownloadSumDroppedReal = 0;
	countDownloadSumDroppedSim = 0;
	countDownloadDroppedReal = 0;
	countDownloadDroppedSim = 0;

	// init simtimes
	delayfirstreal = SIMTIME_ZERO;
	delaylastreal = SIMTIME_ZERO;
	delayfirstsim = SIMTIME_ZERO;
	delaylastsim = SIMTIME_ZERO;

	// train send intervall is defined in ms, not s
	trainSendIntervall = par(kPAR_TRAINSENDINTERVALL).doubleValue() / 1000.0;

	// create topology for getting all ip's
	std::vector<std::string> nedTypeNames;
	nedTypeNames.push_back("src.ned.InternetClient");
	nedTypeNames.push_back("src.ned.MobileClient");
	cTopology* topo = new cTopology("topo");
	topo->extractByNedTypeName(nedTypeNames);
	clientCount = topo->getNumNodes();

	// getting all ip's
	for (int i = 0; i < topo->getNumNodes(); i++) {
		cTopology::Node *node = topo->getNode(i);
		std::string ip;
		if (std::string::npos != node->getModule()->getFullPath().find("mobile")){
			ip = ((MobileNode*)node->getModule())->par(kPAR_IP).stdstringValue();

		} else if (std::string::npos != node->getModule()->getFullPath().find("internet")){
			ip = ((InternetNode*)node->getModule())->par(kPAR_IP).stdstringValue();
		}
		if (!ip.empty()){
			vectorAllNodeIPs.push_back(ip);
		}
	}
	delete (topo);

//	// pushsback for all ip's
//	for (int i = 1; i <= clientCount; i++) {
//		char tmp[10];
//		sprintf(tmp, "127.0.0.%d", i);
//		vectorAllNodeIPs.push_back(tmp);
//	}

	source_ip = par(kPAR_IP).stringValue();
	source_port = par(kPAR_PORT).stringValue();


	// init signals
	if (!isInTestMode) {
		//signals for collecting data (statistics)
		signalDelay 		= registerSignal(kSIGNAL_MOBILE_NODE_TRAVELDELAY);
		signalSendingTime	= registerSignal(kSIGNAL_MOBILE_NODE_SENDINGTIME);
		signalArrivalTime	= registerSignal(kSIGNAL_MOBILE_NODE_ARRIVALTIME);
		signalTrainLength	= registerSignal(kSIGNAL_MOBILE_NODE_TRAINLENGTH);
		signalDropMeasured	= registerSignal(kSIGNAL_MOBILE_NODE_DROPMEASURED);
		signalDropSimulated	= registerSignal(kSIGNAL_MOBILE_NODE_DROPSIMULATED);
	}

	// sned logfiles, when true
	if (par(kPAR_SENDLOGMSG).boolValue()) {
		sendLogFiles();
	}
}

/**
 * default finish
 */
void MobileNode::finish() {
	if (par(kPAR_WRITINGLOGFILENODE).boolValue()) {
		writeLogToHarddrive(NULL, kSTRING_FILE_PACKETSSTATS);
	}
}

/**
 * displays message box with infos at the beginning (currently just in MS WINDOWS)
 */
void MobileNode::showInitialMsgBox() {
	if (par("sendInitialMessage").boolValue() && par("showMsgBox")) {
		char text[255];
		char title[100];

		sprintf(title, "Note for: %s", getFullPath().c_str());

		int simtime = par(kPAR_SIMTIMELIMIT);
		int trainlength = par(kPAR_MAXTRAINLENGTH);
		double trainintervall = par(kPAR_TRAINSENDINTERVALL);

		// build message
		sprintf(text, "modul:\t\t%s\n", getFullPath().c_str());
		sprintf(text, "%scpu time limit:\t%d s", text, simtime);
		sprintf(text, "%s\ntrain length:\t%d", text, trainlength);
		sprintf(text, "%s\ntrain intervall:\t%.1f ms", text, trainintervall);

		// MessageBox(NULL, text, title, MB_OK);
	}
}

/**
 * Read all needed logfiles out of /loggingrun1010/... and will send messages in addiction to
 * the content
 */
void MobileNode::sendLogFiles() {
	long maxPaketCount = INT64_MAX;

	// all file types
	LogFileReader *reader = new LogFileReader;
	std::vector<LogDatarate*> logUpData;
	std::vector<LogDatarate*> logDownData;
	std::vector<LogBbdelay*> logUpBbdelay;
	std::vector<LogBbdelay*> logDownBbdelay;
	std::vector<LogPackets*> logPackets;

	// parse all files
	std::string folder = par(kPAR_INPUTPREFIX).stdstringValue();
	logUpData = reader->parseDatarate(folder + std::string("uploaddatarate.txt"), ",", logUpData, maxPaketCount);
	logDownData	= reader->parseDatarate(folder + std::string("downloaddatarate.txt"), ",", logDownData, maxPaketCount);
	logUpBbdelay = reader->parseBbdelay(folder + std::string("uploadbbdelay.txt"), ",", logUpBbdelay, maxPaketCount);
	logDownBbdelay = reader->parseBbdelay(folder + std::string("downloadbbdelay.txt"), ",", logDownBbdelay, maxPaketCount);
	logPackets = reader->parsePackets(folder + std::string("uploadpackets.txt"), ",", logPackets, maxPaketCount);

	EV<< getFullPath() <<" packets:\t" << logPackets.size() << endl;
	EV<< getFullPath() <<" datarate-download:\t" << logDownData.size() << endl;
	EV<< getFullPath() <<" datarate-upload:\t" << logUpData.size() << endl;
	EV<< getFullPath() <<" bbdelay-download:\t" << logDownBbdelay.size() << endl;
	EV<< getFullPath() <<" bbdelay-upload:\t" << logUpBbdelay.size() << endl;

	// sending all MyPackets
	for (unsigned int i = 0; i < logPackets.size(); i++) {

		char name[10];
		sprintf(name, "Job %d-%d-%d", logPackets.at(i)->seqNr(), logPackets.at(i)->packetNr(),
				logPackets.at(i)->numberOfProbePackets());
		MyPacket* job = new MyPacket(name);
		job->setDroppedSimulated(false);
		job->setBitLength(log2(logPackets.at(i)->payloadsize()));
		job->setPayloadSize(logPackets.at(i)->payloadsize());
		job->setPacketNumber(logPackets.at(i)->packetNr());
		job->setNumberOfProbePackets(logPackets.at(i)->numberOfProbePackets());
		job->setSequenceNumber(logPackets.at(i)->seqNr());
		job->setSourceIP(source_ip.c_str());
		job->setSourcePort(source_port.c_str());
		job->setDestinationIP("127.0.0.2");
		job->setDestinationPort("2345");
		job->setTimestampsArraySize(0);
		job->setEarliestTimeForDelivery(SIMTIME_ZERO);
		job->setDroppedMeasured(logPackets.at(i)->boolDroppedMeasured());
		job->setRecTstampMeasured(logPackets.at(i)->normRecTstamp());
		job->setDelayMeasured(logPackets.at(i)->normRecTstamp() - logPackets.at(i)->normSendTstamp());

		simtime_t delay = logPackets.at(i)->normSendTstamp() - simTime();
		sendMessage(job, delay);
		delete(logPackets.at(i));
	}

	// sending all self message for changing upload datarate
	for (unsigned int i = 0; i < logUpData.size(); i++) {
		Datarate* upData = new Datarate();
		upData->setTimeStamp(logUpData.at(i)->normTstamp());
		upData->setDatarate(logUpData.at(i)->datarate());
		upData->setDatarateSim(logUpData.at(i)->datarateSim());
		upData->setValidDatarate(logUpData.at(i)->valid());
		upData->setDroprate(logUpData.at(i)->droprate());
		upData->setModemMode(logUpData.at(i)->modemMode());
		upData->setSequenceNumber(logUpData.at(i)->seqNr());
		upData->setLongitude(logUpData.at(i)->longitude());
		upData->setLatitude(logUpData.at(i)->latitude());

		cMessage *msg1 = new cMessage(kSTRING_CONTROL_CRSQ);
		cMsgPar* par1 = new cMsgPar(kSTRING_NEWDATARATEPOINTER);
		par1->setPointerValue(upData);
		msg1->addPar(par1);
		sendDelayed(msg1, logUpData.at(i)->normTstamp(), kGATEOUT_CONTROL_TO_CRSQ);
		delete(logUpData.at(i));
	}

	// sending all self message for changing download datarate
	for (unsigned int i = 0; i < logDownData.size(); i++) {
		Datarate* downData = new Datarate();
		downData->setDatarate(logDownData.at(i)->datarate());
		downData->setValidDatarate(logDownData.at(i)->valid());
		downData->setDroprate(logDownData.at(i)->droprate());
		downData->setDatarateSim(logDownData.at(i)->datarateSim());
		downData->setModemMode(logDownData.at(i)->modemMode());
		downData->setSequenceNumber(logDownData.at(i)->seqNr());
		downData->setTimeStamp(logDownData.at(i)->normTstamp());
		downData->setLongitude(logDownData.at(i)->longitude());
		downData->setLatitude(logDownData.at(i)->latitude());

		cMessage *msg2 = new cMessage(kSTRING_CONTROL_CRRQ);
		cMsgPar* par2 = new cMsgPar(kSTRING_NEWDATARATEPOINTER);
		par2->setPointerValue(downData);
		msg2->addPar(par2);
		sendDelayed(msg2, logDownData.at(i)->normTstamp(), kGATEOUT_CONTROL_TO_CRRQ);
		delete(logDownData.at(i));
	}

	// sending all self message for changing upload bbdelay
	for (unsigned int i = 0; i < logUpBbdelay.size(); i++) {
		Bbdelay* upBb = new Bbdelay();
		upBb->setBbdelay(logUpBbdelay.at(i)->backboneDelay());
		upBb->setBbdelaySim(logUpBbdelay.at(i)->backboneDelaySim());
		upBb->setValidBbdelay(logUpBbdelay.at(i)->valid());
		upBb->setModemMode(logUpBbdelay.at(i)->modemMode());
		upBb->setSequenceNumber(logUpBbdelay.at(i)->seqNr());
		upBb->setTimeStamp(logUpBbdelay.at(i)->normTstamp());
		upBb->setLongitude(logUpBbdelay.at(i)->longitude());
		upBb->setLatitude(logUpBbdelay.at(i)->latitude());

		cMessage *msg3 = new cMessage(kSTRING_CONTROL_CDSQ);
		cMsgPar* par3 = new cMsgPar(kSTRING_NEWBBDELAYPOINTER);
		par3->setPointerValue(upBb);
		msg3->addPar(par3);
		sendDelayed(msg3, logUpBbdelay.at(i)->normTstamp(), kGATEOUT_CONTROL_TO_CDSQ);
		delete(logUpBbdelay.at(i));
	}

	// sending all self message for changing download bbdelay
	for (unsigned int i = 0; i < logDownBbdelay.size(); i++) {
		Bbdelay* downBb = new Bbdelay();
		downBb->setBbdelay(logDownBbdelay.at(i)->backboneDelay());
		downBb->setBbdelaySim(logDownBbdelay.at(i)->backboneDelaySim());
		downBb->setValidBbdelay(logDownBbdelay.at(i)->valid());
		downBb->setModemMode(logDownBbdelay.at(i)->modemMode());
		downBb->setSequenceNumber(logDownBbdelay.at(i)->seqNr());
		downBb->setTimeStamp(logDownBbdelay.at(i)->normTstamp());
		downBb->setLongitude(logDownBbdelay.at(i)->longitude());
		downBb->setLatitude(logDownBbdelay.at(i)->latitude());

		cMessage *msg4 = new cMessage(kSTRING_CONTROL_CDRQ);
		cMsgPar* par4 = new cMsgPar(kSTRING_NEWBBDELAYPOINTER);
		par4->setPointerValue(downBb);
		msg4->addPar(par4);
		sendDelayed(msg4, logDownBbdelay.at(i)->normTstamp(), kGATEOUT_CONTROL_TO_CDRQ);
		delete(logDownBbdelay.at(i));
	}

	// garbage collection
	delete (reader);
}

/**
 * default handleMessage method: all incoming message will be received here
 * we will look for the kind of message (self message, unknown, ...) and act like it has to be
 */
void MobileNode::handleMessage(cMessage* msg) {
	MyPacket* job = dynamic_cast<MyPacket*>(msg);

	// handle self message, when we got some
	if (msg->isSelfMessage()) {
		handleSelfMessage(msg);

		// or we handle a kind of MyPacket*
	} else if (job != 0) {
		// set simtimes in MyPacket* for loggng purpose
		simtime_t travelDelay = job->getArrivalTime() - job->getStartTimeForLogging();
		job->setArrivalTimeForLogging(simTime());
		if (job->getDroppedSimulated()) {
			job->setArrivalTime(job->getStartTimeForLogging());
			job->setArrivalTimeForLogging(job->getStartTimeForLogging());
		}

		// set timestamp
		job->setTimestampsArraySize((1 + job->getTimestampsArraySize()));
		char timelog[50];
		sprintf(timelog, "%s %s,%.0f", kSTRING_MOBILENODE, kSTRING_RECEIVED, (simTime().dbl() * kMULTIPLIER_S_TO_NS));
		job->setTimestamps((job->getTimestampsArraySize() - 1), timelog);

		// erase counter on new train
		if (job->getPacketNumber() == 0) {
			countDownloadDroppedSim = 0;
			countDownloadDroppedReal = 0;
		}

		// we got a new message :)
		countPacketSend++;
		if (job->getDroppedSimulated()) {
			countDownloadSumDroppedSim++;
			countDownloadDroppedSim++;
		} else {
			countDownloadReceivedSim++;
		}
		if (job->getDroppedMeasured()) {
			countDownloadSumDroppedReal++;
			countDownloadDroppedReal++;
		} else {
			countDownloadReceivedReal++;
		}

		// console output
		EV<< getFullPath() << " received " << job << " after " << travelDelay << " at " << job->getArrivalTime() << endl;

		// writing log file
		if (!isInTestMode)
			if (par(kPAR_WRITINGLOGFILENODE).boolValue()) {
				writeLogToHarddrive(job, kSTRING_FILE_DOWNLOADPACKETS);
				writeLogToHarddrive(job, kSTRING_FILE_DOWNLOADRECEIVED);
				writeLogToHarddrive(job, kSTRING_FILE_DOWNLOADPACKETTIMESTAMPS);
				writeLogToHarddrive(job, kSTRING_FILE_PACKETS);
				writeLogToHarddrive(job, kSTRING_FILE_PACKETSDOWNLOAD);


				// setting delays to last non dropped packet in train
				if (!job->getDroppedMeasured()){
					delete lastInTrainNonDroppedMeasuredPacket;
					lastInTrainNonDroppedMeasuredPacket = job->dup();
					if (!firstNonDroppedMeasuredPacket){
						delete firstInTrainNonDroppedMeasuredPacket;
						firstInTrainNonDroppedMeasuredPacket = job->dup();
						firstNonDroppedMeasuredPacket = true;
					}
				}
				if (!job->getDroppedSimulated()){
					delete lastInTrainNonDroppedSimulatedPacket;
					lastInTrainNonDroppedSimulatedPacket = job->dup();
					if (!firstNonDroppedSimulatedPacket){
						delete firstInTrainNonDroppedSimulatedPacket;
						firstInTrainNonDroppedSimulatedPacket = job->dup();
						firstNonDroppedSimulatedPacket = true;
					}
				}

				// first packet
				if (job->getPacketNumber() == 0) {
					writeLogToHarddrive(job, kSTRING_FILE_DOWNLOADSTATS_FIRSTDIFF);
					delayfirstreal = job->getRecTstampMeasured() - job->getStartTimeForLogging();
					delayfirstsim = simTime() - job->getStartTimeForLogging();

					writeLogToHarddrive(job, kSTRING_FILE_DOWNLOAD_REALCDF);
					writeLogToHarddrive(job, kSTRING_FILE_DOWNLOAD_SIMCDF);
				}


				// last packet
				if (job->getPacketNumber() + 1 == job->getNumberOfProbePackets()) {
					writeLogToHarddrive(job, kSTRING_FILE_DOWNLOADSTATS_LASTDIFF);
					delaylastreal = job->getRecTstampMeasured() - job->getStartTimeForLogging();
					delaylastsim = simTime() - job->getStartTimeForLogging();

					// correcture when dropped
					// dropped packets got sendtime = receivetime, so zero delay
					if (delayfirstreal == SIMTIME_ZERO){
						delayfirstreal = firstInTrainNonDroppedMeasuredPacket->getRecTstampMeasured() - firstInTrainNonDroppedMeasuredPacket->getStartTimeForLogging();
					}
					if (delayfirstsim == SIMTIME_ZERO){
						delayfirstsim = simTime() - firstInTrainNonDroppedSimulatedPacket->getStartTimeForLogging();
					}
					if (job->getDroppedMeasured()){
						delaylastreal = lastInTrainNonDroppedMeasuredPacket->getRecTstampMeasured() - lastInTrainNonDroppedMeasuredPacket->getStartTimeForLogging();
					}
					if (job->getDroppedSimulated()){
						delaylastsim = simTime() - lastInTrainNonDroppedSimulatedPacket->getStartTimeForLogging();
					}

					writeLogToHarddrive(job, kSTRING_FILE_DOWNLOADSTATS);

					// emiting signals
					emit(signalDropMeasured, countDownloadDroppedReal);
					emit(signalDropSimulated, -countDownloadDroppedSim);

					// refresh booleans for getting first undropped packets in train
					firstNonDroppedMeasuredPacket = false;
					firstNonDroppedSimulatedPacket = false;
				}
			}

		// emitting signals
		emit(signalDelay, travelDelay);
		emit(signalArrivalTime, job->getArrivalTime());
		emit(signalSendingTime, job->getStartTimeForLogging());

		// delete references
		delete (msg);

		// last chance...drop it
	} else if (!isInTestMode) {
		EV<< getFullPath() << "packet with name: " << msg->getFullName()
		<< ", type (" << msg << "), at " << simTime()<< endl;

		// delete references
//       		delete (job);
		delete (msg);
	}
}

		/**
		 * just handling self messages
		 * @param msg, self cMessage*
		 */
void MobileNode::handleSelfMessage(cMessage *msg) {
	// self message for starting new packet train?
	if (strcmp(msg->getName(), kSTRING_STARTTRAIN) == 0) {
		sendPacketTrain();
		// check, if a new self message should be scheduled
		if (simTime() < par(kPAR_SIMTIMELIMIT).doubleValue() - trainSendIntervall) {
			EV<< getFullPath() << " next packet train at simtime: " << simTime()+trainSendIntervall;
			EV << ", current simtime " << simTime() << endl;
			scheduleAt(simTime() + trainSendIntervall, selfMsgStartTrain);
		}
		// self messge for changing delays?
	} else if (strcmp(msg->getName(), kPAR_CHANGEPACKETSDELAY) == 0 && par(kPAR_CHANGEPACKETSDELAY).boolValue()) {
		simtime_t tmp = dblrand()*3;
		scheduleAt(simTime() + tmp, selfMsgChangeDelays);

		bubble("DELAY CHANGE");
		// testing, random values
		bool validdatarate = dblrand() > 0.05 ? "true" : "false";
		bool validbbdelay = dblrand() > 0.05 ? "true" : "false";
		double datarate = (20 + intrand(500)) * 1000;
		double droprate = dblrand() * dblrand() * dblrand();
		simtime_t bbdelay1 = dblrand()/1000.0;
		simtime_t bbdelay2 = dblrand()/1000.0;
		simtime_t tstamp = simTime();
		std::string modemMode = "";

		// create new down/up download/bbdelays
		Datarate* d_down= new Datarate(datarate, 0.0, 0.0, validdatarate, droprate, modemMode, ++controlPacketsSequenceNumber, tstamp);
		Datarate* d_up = new Datarate(datarate/4, 0.0, 0.0, validdatarate, droprate/2, modemMode, ++controlPacketsSequenceNumber, tstamp);
		Bbdelay* b_down = new Bbdelay (bbdelay1, 0.0, 0.0, validbbdelay, modemMode, ++controlPacketsSequenceNumber, tstamp);
		Bbdelay* b_up = new Bbdelay (bbdelay2, 0.0, 0.0, validbbdelay, modemMode, ++controlPacketsSequenceNumber, tstamp);

		// and set the values
		setUpDatarate(d_up);
		setUpBbdelay(b_up);
		setDownBbdelay(b_down);
		setDownDatarate(d_down);

		// or unknown self message?
	} else {
		EV << getFullPath() << " unknown self message: " << msg->getFullName() << " , type " << msg << " , at " << simTime()<< endl;
		delete(msg);
	}
}

			/**
			 * Creates output path, so folder and file. If choosen, even localtime will be included.
			 * @param filename
			 * @return
			 */
std::string MobileNode::buildAndMakePath(std::string filename) {
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
	return buff;
}

/**
 * Will log some information in logfiles. Kind of information depens on given filename
 * @param job for logging
 * @param filename for saving
 */
void MobileNode::writeLogToHarddrive(MyPacket *job, std::string filename) {
	outputPath = buildAndMakePath(filename);

	// choose functions...depends on given filename
	if (filename.compare(kSTRING_FILE_DOWNLOADPACKETS) == 0) {
		logwriter->writeLogfileForPackets(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_DOWNLOADRECEIVED) == 0) {
		logwriter->writeLogfileForPacketsReceived(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_DOWNLOADPACKETTIMESTAMPS) == 0) {
		logwriter->writeLogfileForPacketsTimestamps(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_UPLOADSEND) == 0) {
		logwriter->writeLogfileForPacketsSend(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_UPLOADPACKETS) == 0) {
		logwriter->writeLogfileForPackets(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_PACKETS) == 0) {
		logwriter->writeLogfileForPacketsComplete(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_PACKETSDOWNLOAD) == 0) {
		logwriter->writeLogfileForPacketsComplete(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_PACKETSSTATS) == 0) {
		int counterArray[5];
		counterArray[0] = countPacketSend;
		counterArray[1] = countDownloadReceivedReal;
		counterArray[2] = countDownloadReceivedSim;
		counterArray[3] = countDownloadSumDroppedReal;
		counterArray[4] = countDownloadSumDroppedSim;
		logwriter->writeLogfileForPacketsStats(outputPath, kSTRING_DOWNLOAD, counterArray);
	} else if (filename.compare(kSTRING_FILE_DOWNLOADSTATS) == 0) {
		double counterArray[8];
		counterArray[0] = delayfirstreal.dbl();
		counterArray[1] = delaylastreal.dbl();
		counterArray[2] = delayfirstsim.dbl();
		counterArray[3] = delaylastsim.dbl();
		counterArray[4] = countDownloadDroppedReal;
		counterArray[5] = countDownloadDroppedSim;
		counterArray[6] = countDownloadSumDroppedReal;
		counterArray[7] = countDownloadSumDroppedSim;
		logwriter->writeLogfileForTrainsStats(outputPath, job, kSTRING_DOWNLOAD, counterArray);
	} else if (filename.compare(kSTRING_FILE_DOWNLOADSTATS_FIRSTDIFF)
			|| filename.compare(kSTRING_FILE_DOWNLOADSTATS_LASTDIFF)) {
		logwriter->writeLogfileForPacketsStatsDiff(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_DOWNLOAD_REALCDF) || filename.compare(kSTRING_FILE_DOWNLOAD_SIMCDF)) {
		logwriter->writeLogfileForPacketsCDF(outputPath, job);
	}
}

/**
 * Send inital message for sending train-bursts
 */
void MobileNode::sendInitialMessage() {
	if (par(kPAR_SENDINITMSG).boolValue() && !par(kPAR_SENDLOGMSG).boolValue()) {
		scheduleAt(kSECOND, selfMsgStartTrain);
		EV<< "Message's name structure: 'job-%d-%d-%d|%d' with 'sequenceNr, packetNr, nrOfProbePackets, , msgCountSent'" << endl;
	}
	if (par(kPAR_CHANGEPACKETSDELAY).boolValue() && !par(kPAR_SENDLOGMSG).boolValue()) {
		scheduleAt(simTime()+0.1, selfMsgChangeDelays);
	}
}

		/**
		 * Changes the datarate in upload queue (CRSQ)
		 * @param newDatarate, Datarate*
		 */
void MobileNode::setUpDatarate(Datarate* newDatarate) {
	cMessage *msg = new cMessage(kSTRING_CONTROL_CRSQ);
	cMsgPar* par = new cMsgPar(kSTRING_NEWDATARATEPOINTER);
	par->setPointerValue(newDatarate);
	msg->addPar(par);
	sendDelayed(msg, SIMTIME_ZERO, kGATEOUT_CONTROL_TO_CRSQ);
}

/**
 * Changes the backbone delay in upload queue (CDSQ)
 * @param newBbdelay, Bbdelay*
 */
void MobileNode::setUpBbdelay(Bbdelay* newBbdelay) {
	cMessage *msg = new cMessage(kSTRING_CONTROL_CDSQ);
	cMsgPar* par = new cMsgPar(kSTRING_NEWBBDELAYPOINTER);
	par->setPointerValue(newBbdelay);
	msg->addPar(par);
	sendDelayed(msg, SIMTIME_ZERO, kGATEOUT_CONTROL_TO_CDSQ);
}

/**
 * Changes the datarate in upload queue (CRRQ)
 * @param newDatarate, Datarate*
 */
void MobileNode::setDownDatarate(Datarate* newDatarate) {
	cMessage *msg = new cMessage(kSTRING_CONTROL_CRRQ);
	cMsgPar* par = new cMsgPar(kSTRING_NEWDATARATEPOINTER);
	par->setPointerValue(newDatarate);
	msg->addPar(par);
	sendDelayed(msg, SIMTIME_ZERO, kGATEOUT_CONTROL_TO_CRRQ);
}

/**
 * Changes the backbone delay in upload queue (CDRQ)
 * @param newBbdelay, Bbdelay*
 */
void MobileNode::setDownBbdelay(Bbdelay* newBbdelay) {
	cMessage *msg = new cMessage(kSTRING_CONTROL_CDRQ);
	cMsgPar* par = new cMsgPar(kSTRING_NEWBBDELAYPOINTER);
	par->setPointerValue(newBbdelay);
	msg->addPar(par);
	sendDelayed(msg, SIMTIME_ZERO, kGATEOUT_CONTROL_TO_CDRQ);

}

/**
 * Sends a new packet traing with random length and random destination
 */
void MobileNode::sendPacketTrain() {
	sequenceNumber++;

	char name[20];

	// sending burst
	int burst = intuniform(2, par(kPAR_MAXTRAINLENGTH));

	// getting the IP via cPar*
	const char *dest_ip = par(kPAR_DESITNATIONIP).stringValue();

	if (strlen(dest_ip) == 0) {
		dest_ip = vectorAllNodeIPs.at((int) (dblrand() * clientCount)).c_str();
		while (source_ip.compare(dest_ip) == 0) {
			dest_ip = vectorAllNodeIPs.at((int) (dblrand() * clientCount)).c_str();
		}
	}
	// emit burst length as signal
	emit(signalTrainLength, burst);

	EV<< getFullPath() << " sending " << burst << " packets to: " << dest_ip;
	EV<< ", size: " << burst << "*" << kUMTS_PACKETSIZE << "Byte";
	EV<< "destination: " << dest_ip << endl;

	// send all packets, back to back
	for (int i = 1; i <= burst; i++) {
		sprintf(name, "job-%d-%d-%d|%d", sequenceNumber, i, burst, countMsgSend);

		// filling the package
		MyPacket* job = new MyPacket(name);
		job->setDroppedSimulated(false);
		job->setBitLength(log2(kUMTS_PACKETSIZE));
		job->setPayloadSize(kUMTS_PACKETSIZE);
		job->setPacketNumber(i);
		job->setNumberOfProbePackets(burst);
		job->setSequenceNumber(sequenceNumber);
		job->setSourceIP(source_ip.c_str());
		job->setSourcePort(source_port.c_str());
		job->setDestinationIP(dest_ip);
		job->setDestinationPort("2345");
		job->setTimestampsArraySize(0);
		job->setEarliestTimeForDelivery(SIMTIME_ZERO);

		sendMessage(job, SIMTIME_ZERO);
	}

	// output in tkenv
	bubble("send train");
}

void MobileNode::sendMessage(MyPacket* job, simtime_t delayUntilNow) {
	// getting the next available time for sending
//	simtime_t finTime = gate("out")->getTransmissionChannel()->getTransmissionFinishTime();
//	delay = finTime >= currentSimtime ? finTime - currentSimtime : 0;

// add packet timestamp
	job->setTimestampsArraySize((1 + job->getTimestampsArraySize()));
	char namelog[30];
	sprintf(namelog, "%d,%d,%d", job->getSequenceNumber(), job->getPacketNumber(), job->getNumberOfProbePackets());
	job->setTimestamps((job->getTimestampsArraySize() - 1), namelog);

	// add send time
	job->setTimestampsArraySize((1 + job->getTimestampsArraySize()));
	char timelog[50];
	sprintf(timelog, "%s %s %f", kSTRING_MOBILENODE, kSTRING_SEND,
			(simTime() + delayUntilNow).dbl() * kMULTIPLIER_S_TO_NS);
	job->setTimestamps((job->getTimestampsArraySize() - 1), timelog);
	job->setStartTimeForLogging(simTime() + delayUntilNow);
	job->setSendingTimeForLogging(simTime() + delayUntilNow);

	// writing log file
	if (!isInTestMode){
		if (par(kPAR_WRITINGLOGFILENODE).boolValue()) {
			writeLogToHarddrive(job, kSTRING_FILE_UPLOADSEND);
			writeLogToHarddrive(job, kSTRING_FILE_UPLOADPACKETS);
		}
	}

	countMsgSend++;
//	simtime_t finishTime = gate("out")->getTransmissionChannel()->getTransmissionFinishTime();
//	double ft, st, dt;
//	ft = finishTime.dbl();
//	st = simTime().dbl();
//	dt = delayUntilNow.dbl();
//	if (finishTime < 10){
//		delayUntilNow += finishTime > simTime() ? finishTime - simTime() : 0;
		sendDelayed(job, delayUntilNow, "out");
//	} else {
//		EV << "getTransmissionFinishTime time exploding: " << finishTime << " for " << job << endl;
//	}
}

}
;

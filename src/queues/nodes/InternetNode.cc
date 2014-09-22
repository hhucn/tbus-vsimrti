/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "InternetNode.h"

namespace projekt {

Define_Module(InternetNode)
;

/**
 * default constructor
 */
InternetNode::InternetNode() {
	isInTestMode = false;
}

/**
 * Default constructor for test modes
 * @param testMode, true for testmodes
 */
InternetNode::InternetNode(bool testMode) {
	isInTestMode = testMode;
}

/**
 * default destrcutor
 */
InternetNode::~InternetNode() {
	delete logwriter;

	delete firstInTrainNonDroppedMeasuredPacket;
	delete firstInTrainNonDroppedSimulatedPacket;
	delete lastInTrainNonDroppedMeasuredPacket;
	delete lastInTrainNonDroppedSimulatedPacket;
}

/**
 * default init
 */
void InternetNode::initialize() {
	// defining signals for logging
	signalDelay = registerSignal(kSIGNAL_INET_NODE_TRAVELDELAY);
	logwriter = new LogFileWriter();

	// init boolean
	firstNonDroppedMeasuredPacket = false;
	firstNonDroppedSimulatedPacket = false;

	// init counter
	countPacketSend = 0;
	countUploadReceivedReal = 0;
	countUploadReceivedSim = 0;
	countUploadSumDroppedReal = 0;
	countUploadSumDroppedSim = 0;
	countUploadDroppedReal = 0;
	countUploadDroppedSim = 0;

	// init simtimes
	delayfirstreal = SIMTIME_ZERO;
	delaylastreal = SIMTIME_ZERO;
	delayfirstsim = SIMTIME_ZERO;
	delaylastsim = SIMTIME_ZERO;

	source_ip = par(kPAR_IP).stringValue();
	source_port = par(kPAR_PORT).stringValue();

	if (par(kPAR_SENDLOGMSG).boolValue()) {
		sendLogFiles();
	}

	// init signals
	if (!isInTestMode) {
		//signals for collecting data (statistics)
		signalDropMeasured		= registerSignal(kSIGNAL_INET_NODE_DROPMEASURED);
		signalDropSimulated		= registerSignal(kSIGNAL_INET_NODE_DROPSIMULATED);
	}
}

/**
 * default finish
 */
void InternetNode::finish() {
	if (par(kPAR_WRITINGLOGFILENODE).boolValue()) {
		writeLogToHarddrive(NULL, kSTRING_FILE_PACKETSSTATS);
	}
}

/**
 * Read all needed logfiles out of /loggingrun1010/... and will send messages in addiction to
 * the content
 */
void InternetNode::sendLogFiles() {
	long maxPaketCount = INT64_MAX;

	// all file types
	std::vector<LogPackets*> logPackets;
	LogFileReader *reader = new LogFileReader;

	// parse all files
	std::string folder = par(kPAR_INPUTPREFIX).stdstringValue();
	logPackets = reader->parsePackets(folder + std::string("downloadpackets.txt"), ",", logPackets, maxPaketCount);

	EV<< getFullPath() <<" packets:\t" << logPackets.size() << endl;

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
		job->setDestinationIP("127.0.0.1");
		job->setDestinationPort("2345");
		job->setTimestampsArraySize(0);
		job->setEarliestTimeForDelivery(SIMTIME_ZERO);
		job->setDroppedMeasured(logPackets.at(i)->boolDroppedMeasured());
		job->setRecTstampMeasured(logPackets.at(i)->normRecTstamp());
		job->setDelayMeasured(logPackets.at(i)->normRecTstamp() - logPackets.at(i)->normSendTstamp());

		simtime_t delay = logPackets.at(i)->normSendTstamp() - simTime();
		sendMessage(job, delay);
		delete (logPackets.at(i));
	}

	delete (reader);

}

/**
 * Called by simulation. Handles all incoming messages
 * @param msg
 */
void InternetNode::handleMessage(cMessage *msg) {
	MyPacket* job = dynamic_cast<MyPacket*>(msg);

	// checking, weather incoming message is instance of MyPacket*
	if (job != 0) {
		simtime_t travelDelay = job->getArrivalTime() - job->getStartTimeForLogging();

		// erase counter on new train
		if (job->getPacketNumber() == 0) {
			countUploadDroppedSim = 0;
			countUploadDroppedReal = 0;
		}

		// count message and set time, when packet was dropped
		countPacketSend++;
		job->setArrivalTimeForLogging(simTime());
		if (job->getDroppedSimulated()) {
			job->setArrivalTime(job->getStartTimeForLogging());
			job->setArrivalTimeForLogging(job->getStartTimeForLogging());
			countUploadSumDroppedSim++;
			countUploadDroppedSim++;
		} else {
			countUploadReceivedSim++;
		}

		if (job->getDroppedMeasured()) {
			countUploadSumDroppedReal++;
			countUploadDroppedReal++;
		} else {
			countUploadReceivedReal++;
		}

		// set timestamp
		job->setTimestampsArraySize((1 + job->getTimestampsArraySize()));
		char timelog[50];
		sprintf(timelog, "%s %s,%.0f", kSTRING_INTERNETNODE, kSTRING_RECEIVED, simTime().dbl() * kMULTIPLIER_S_TO_NS);
		job->setTimestamps((job->getTimestampsArraySize() - 1), timelog);

		// output
		if (!isInTestMode) {
			EV<< getFullPath() << " received " << job << " after " << travelDelay << " at " << job->getArrivalTime() << endl;
		}

			// writing log file
		if (!isInTestMode) {
			if (par(kPAR_WRITINGLOGFILENODE).boolValue()) {
				writeLogToHarddrive(job, kSTRING_FILE_UPLOADPACKETS);
				writeLogToHarddrive(job, kSTRING_FILE_UPLOADRECEIVED);
				writeLogToHarddrive(job, kSTRING_FILE_UPLOADPACKETTIMESTAMPS);
				writeLogToHarddrive(job, kSTRING_FILE_PACKETS);
				writeLogToHarddrive(job, kSTRING_FILE_PACKETSUPLOAD);


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
					writeLogToHarddrive(job, kSTRING_FILE_UPLOADSTATS_FIRSTDIFF);

					delayfirstreal = job->getRecTstampMeasured() - job->getStartTimeForLogging();
					delayfirstsim = simTime() - job->getStartTimeForLogging();

					writeLogToHarddrive(job, kSTRING_FILE_UPLOAD_REALCDF);
					writeLogToHarddrive(job, kSTRING_FILE_UPLOAD_SIMCDF);
				}

				// last packet
				if (job->getPacketNumber() + 1 == job->getNumberOfProbePackets()) {
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
					if (job->getDroppedMeasured() && lastInTrainNonDroppedMeasuredPacket){
						delaylastreal = lastInTrainNonDroppedMeasuredPacket->getRecTstampMeasured() - lastInTrainNonDroppedMeasuredPacket->getStartTimeForLogging();
					}
					if (job->getDroppedSimulated() && lastInTrainNonDroppedSimulatedPacket){
						delaylastsim = simTime() - lastInTrainNonDroppedSimulatedPacket->getStartTimeForLogging();
					}

					writeLogToHarddrive(job, kSTRING_FILE_UPLOADSTATS);
					writeLogToHarddrive(job, kSTRING_FILE_UPLOADSTATS_LASTDIFF);

					// emiting signals
					emit(signalDropMeasured, countUploadDroppedReal);
					emit(signalDropSimulated, -countUploadDroppedSim);

					// refresh booleans for getting first undropped packets in train
					firstNonDroppedMeasuredPacket = false;
					firstNonDroppedSimulatedPacket = false;
				}
			}
		}

		// emitting signals
		emit(signalDelay, travelDelay);

		// delete it
		delete (msg);

		// package is unkown, log and delete it
	} else if (!isInTestMode) {
		EV<< getFullPath() << "packet with name: " << msg->getFullName()
		<< ", type (" << msg << "), at " << simTime()<< endl;

		// delete references
		delete (msg);
	}
}

		/**
		 * Creates output path, so folder and file. If choosen, even localtime will be included.
		 * @param filename
		 * @return
		 */
std::string InternetNode::buildAndMakePath(std::string filename) {
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
void InternetNode::writeLogToHarddrive(MyPacket *job, std::string filename) {
	outputPath = buildAndMakePath(filename);

	// compare given filename with #defines for choosing right function
	if (filename.compare(kSTRING_FILE_UPLOADPACKETS) == 0) {
		logwriter->writeLogfileForPackets(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_UPLOADRECEIVED) == 0) {
		logwriter->writeLogfileForPacketsReceived(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_UPLOADPACKETTIMESTAMPS) == 0) {
		logwriter->writeLogfileForPacketsTimestamps(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_DOWNLOADSEND) == 0) {
		logwriter->writeLogfileForPacketsSend(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_PACKETS) == 0) {
		logwriter->writeLogfileForPacketsComplete(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_PACKETSUPLOAD) == 0) {
		logwriter->writeLogfileForPacketsComplete(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_PACKETSSTATS) == 0) {
		// saving all counter in array
		int counterArray[5];
		counterArray[0] = countPacketSend;
		counterArray[1] = countUploadReceivedReal;
		counterArray[2] = countUploadReceivedSim;
		counterArray[3] = countUploadSumDroppedReal;
		counterArray[4] = countUploadSumDroppedSim;
		// add two whitespaces to direction (it just looks nicer)
		char s[10];
		sprintf(s, "%s  ", kSTRING_UPLOAD);
		std::string direction = s;
		logwriter->writeLogfileForPacketsStats(outputPath, direction, counterArray);
	} else if (filename.compare(kSTRING_FILE_UPLOADSTATS) == 0) {
		double counterArray[8];
		counterArray[0] = delayfirstreal.dbl();
		counterArray[1] = delaylastreal.dbl();
		counterArray[2] = delayfirstsim.dbl();
		counterArray[3] = delaylastsim.dbl();
		counterArray[4] = countUploadDroppedReal;
		counterArray[5] = countUploadDroppedSim;
		counterArray[6] = countUploadSumDroppedReal;
		counterArray[7] = countUploadSumDroppedSim;
		logwriter->writeLogfileForTrainsStats(outputPath, job, kSTRING_UPLOAD, counterArray);
	} else if (filename.compare(kSTRING_FILE_UPLOADSTATS_FIRSTDIFF)
			|| filename.compare(kSTRING_FILE_UPLOADSTATS_LASTDIFF)) {
		logwriter->writeLogfileForPacketsStatsDiff(outputPath, job);
	} else if (filename.compare(kSTRING_FILE_UPLOAD_REALCDF) || filename.compare(kSTRING_FILE_UPLOAD_SIMCDF)) {
		logwriter->writeLogfileForPacketsCDF(outputPath, job);
	}
}

void InternetNode::sendMessage(MyPacket* job, simtime_t delayUntilNow) {
	// add paket stamp
	job->setTimestampsArraySize((1 + job->getTimestampsArraySize()));
	char namelog[30];
	sprintf(namelog, "%d,%d,%d", job->getSequenceNumber(), job->getPacketNumber(), job->getNumberOfProbePackets());
	job->setTimestamps((job->getTimestampsArraySize() - 1), namelog);

	// add send time
	job->setTimestampsArraySize((1 + job->getTimestampsArraySize()));
	char timelog[50];
	sprintf(timelog, "%s %s,%.0f", kSTRING_INTERNETNODE, kSTRING_SEND,
			(simTime() + delayUntilNow).dbl() * kMULTIPLIER_S_TO_NS);
	job->setTimestamps((job->getTimestampsArraySize() - 1), timelog);
	job->setStartTimeForLogging(delayUntilNow + simTime());
	job->setSendingTimeForLogging(delayUntilNow + simTime());

	// writing log file
	if (par(kPAR_WRITINGLOGFILENODE).boolValue()) {
		writeLogToHarddrive(job, kSTRING_FILE_DOWNLOADSEND);
	}

//	simtime_t finishTime = gate("out")->getChannel()->getTransmissionFinishTime();
//	delayUntilNow += finishTime > simTime() ? finishTime - simTime() : 0;
	sendDelayed(job, delayUntilNow, "out");
}

}
/* namespace projekt */

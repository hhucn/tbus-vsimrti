/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "LogFileWriter.h"

namespace projekt {

/**
 * default constructor
 */
LogFileWriter::LogFileWriter() {
}

/**
 * default destructor
 */
LogFileWriter::~LogFileWriter() {
}

/**
 * Checks for an empty file. The file is empty, when there are no printable signs
 * @param local path to file
 * @returns true, when file is empty
 */
bool LogFileWriter::is_textfile_empty(std::string filename) {
	std::string s;
	std::ifstream f(filename.c_str(), std::ios::binary);

	// Check for UTF-8 BOM
	if (f.peek() == 0xEF) {
		f.get();
		if (f.get() != 0xBB) return false;
		if (f.get() != 0xBF) return false;
	}

	// Scan every line of the file for non-whitespace characters
	while (getline(f, s)) {
		if (s.find_first_not_of(" \t\n\v\f\n" // whitespace
						"\0\xFE\xFF"// non-printing (used in various Unicode encodings)
		) != std::string::npos) return false;
	}

	// If we get this far, then the file only contains whitespace
	// (or its size is zero)
	return true;
}

/**
 * Writes some information on harddrive
 * @param ...
 */
void LogFileWriter::writeLogToHarddrive(MyPacket* job, std::string folder, std::string filename,
		Bbdelay* currentBbdelay, Datarate* currentDatarate) {
	if (outputPath.empty() || localTime.empty()) {
		mkdir(folder.c_str(), S_IRWXU);

		outputPath[0] = 0;
		localTime[0] = 0;

		Localtime *lt = new Localtime();
		localTime = lt->currentLocaltimeForLogfile(false).c_str();
		delete (lt);
		char buff[50];
		sprintf(buff, "%s%s%s", folder.c_str(), localTime.c_str(), filename.c_str());
		outputPath = buff;
//			EV << getFullPath() << " build output path: " << outputPath << endl;
	}

	// TODO: NOT COMPLETED: MAYBE SOME TIME

//	if (filename.compare(kSTRING_FILE_CRSQ_SEND) == 0) {						writeLogfileForPacketsSendQueue(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_CDSQ_SEND) == 0) {					writeLogfileForPacketsSendQueue(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_CRRQ_SEND) == 0) {					writeLogfileForPacketsSendQueue(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_CDRQ_SEND) == 0) {					writeLogfileForPacketsSendQueue(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_CRSQ_RECEIVED) == 0) {				writeLogfileForPacketsReceivedQueue(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_CDSQ_RECEIVED) == 0) {				writeLogfileForPacketsReceivedQueue(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_CRRQ_RECEIVED) == 0) {				writeLogfileForPacketsReceivedQueue(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_CDRQ_RECEIVED) == 0) {				writeLogfileForPacketsReceivedQueue(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_UPLOADDATARATE) == 0) {			writeLogfileForDatarate(outputPath, currentDatarate);
//	} else if (filename.compare(kSTRING_FILE_UPLOADBBDELAY) == 0) {				writeLogfileForBbdelay(outputPath, currentBbdelay);
//	} else if (filename.compare(kSTRING_FILE_DOWNLOADBBDELAY) == 0) 	{		writeLogfileForBbdelay(outputPath, currentBbdelay);
//	} else if (filename.compare(kSTRING_FILE_UPLOADPACKETS) == 0) {				writeLogfileForPackets(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_UPLOADRECEIVED) == 0) {			writeLogfileForPacketsReceived(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_DOWNLOADRECEIVED) == 0) {			writeLogfileForPacketsReceived(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_UPLOADSEND) == 0) {				writeLogfileForPacketsSend(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_DOWNLOADSEND) == 0) {				writeLogfileForPacketsSend(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_DOWNLOADDATARATE) == 0) {			writeLogfileForDatarate(outputPath, currentDatarate);
//	} else if (filename.compare(kSTRING_FILE_UPLOADPACKETTIMESTAMPS) == 0) {	writeLogfileForPacketsTimestamps(outputPath, job);
//	} else if (filename.compare(kSTRING_FILE_DOWNLOADPACKETTIMESTAMPS) == 0) {	writeLogfileForPacketsTimestamps(outputPath, job);
//	}
}

/**
 * Logs all informations from a Bbdelay
 * @param path for the file
 * @param bbdelay, which should be logged
 */
void LogFileWriter::writeLogfileForBbdelay(std::string path, Bbdelay* bbdelay) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all TStamps are ns\n";
			outputFile << "#NormTstamp, Latitude, Longitude, BackBoneDelay, BackBoneDelaySim, ";
			outputFile << "BoolValidBbDelay, Delay, Droprate, SeqNr, ModemMode, Trainsize, Packetsize\n";
		}

		long int bTime = (simTime().dbl() *  kMULTIPLIER_S_TO_NS);
		long int dTime = (bbdelay->bbdelay().dbl() *  kMULTIPLIER_S_TO_NS);
		long int dsTime = (bbdelay->bbdelaySim().dbl() *  kMULTIPLIER_S_TO_NS);
		long int dlTime = (bbdelay->delay().dbl() *  kMULTIPLIER_S_TO_NS);

		outputFile << bTime << ",";
		outputFile << bbdelay->latitude() << ",";
		outputFile << bbdelay->longitude() << ",";
		outputFile << dTime << ",";
		outputFile << dsTime << ",";
		outputFile << (bbdelay->validbbdelay() ? "true" : "false") << ",";
		outputFile << dlTime << ",";
		outputFile << (bbdelay->droprate() < kEPS7 ? 0 : bbdelay->droprate()) << ",";
		outputFile << bbdelay->sequenceNumber() << ",";
		outputFile << bbdelay->modemMode() << ",";
		outputFile << bbdelay->cMsgNumberOfProbePackets() << ",";
		outputFile << bbdelay->cMsgPayloadSize() << "\n";
	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	////outputFile.flush();
	outputFile.close();
}

/**
 * Logs all informations from a Datarate
 * @param path for the file
 * @param datarate, which should be logged
 */
void LogFileWriter::writeLogfileForDatarate(std::string path, Datarate* datarate) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all TStamps are ns\n";
			outputFile << "#NormTstamp, Latitude, Longitude, Data Rate, Data Rate Sim, ";
			outputFile << "BoolValidDr, Droprate, SeqNr, ModemMode, Trainsize, Packetsize\n";
		}

		long int sTime = (simTime().dbl() *  kMULTIPLIER_S_TO_NS);

		outputFile << sTime << ",";
		outputFile << datarate->latitude() << ",";
		outputFile << datarate->longitude() << ",";
		outputFile << datarate->datarate() / kDIVIDER_KB_TO_B << ",";
		outputFile << datarate->datarateSim() / kDIVIDER_KB_TO_B << ",";
		outputFile << (datarate->validdatarate() ? "true" : "false") << ",";
		outputFile << datarate->droprate() << ",";
		outputFile << datarate->sequenceNumber() << ",";
		outputFile << datarate->modemMode() << ",";
		outputFile << datarate->cMsgNumberOfProbePackets() << ",";
		outputFile << datarate->cMsgPayloadSize() << "\n";

	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Logs all informations from a MyPacket
 * @param path for the file
 * @param packet, which should be logged
 */
void LogFileWriter::writeLogfileForPackets(std::string path, MyPacket* packet) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all TStamps are ns\n";
			outputFile << "#a dropped packet implys recTstamp = sendTstamp\n";
			outputFile
					<< "#NormStartTstamp, NormRecTstamp, SeqNr, PacketNr, NumberOfProbePackets, Payloadsize, BoolDropped\n";
		}

		long int aTime = (packet->getStartTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);
		long int sTime = (packet->getArrivalTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);

		outputFile << aTime << ",";
		outputFile << sTime << ",";

		outputFile << packet->getSequenceNumber() << ",";
		outputFile << packet->getPacketNumber() << ",";
		outputFile << packet->getNumberOfProbePackets() << ",";
		outputFile << packet->getPayloadSize() << ",";
		outputFile << (packet->getDroppedSimulated() ? "true" : "false") << "\n";
	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Logs all informations from a MyPacket
 * @param path for the file
 * @param packet, which should be logged
 */
void LogFileWriter::writeLogfileForPacketsReceived(std::string path, MyPacket* packet) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all TStamps are ns\n";
			outputFile << "#a dropped packet implys recTstamp = sendTstamp\n";
			outputFile << "#NormSendTstamp, NormRecTstamp, Delay, SeqNr, PacketNr, Payloadsize, NumberOfProbePackets\n";
		}

		long int aTime = (packet->getStartTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);
		long int sTime = (packet->getArrivalTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);
		long int dTime = ((packet->getArrivalTimeForLogging() - packet->getStartTimeForLogging()).dbl() *  kMULTIPLIER_S_TO_NS);

		outputFile << aTime << ",";
		outputFile << sTime << ",";
		outputFile << dTime << ",";

		outputFile << packet->getSequenceNumber() << ",";
		outputFile << packet->getPacketNumber() << ",";
		outputFile << packet->getPayloadSize() << ",";
		outputFile << packet->getNumberOfProbePackets() << "\n";
	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Logs all informations from a MyPacket
 * @param path for the file
 * @param packet, which should be logged
 */
void LogFileWriter::writeLogfileForPacketsSend(std::string path, MyPacket* packet) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all TStamps are ns\n";
			outputFile << "#a dropped packet implys rectstamp = sendTstamp\n";
			outputFile << "#NormTstamp, SeqNr, PacketNr, Payloadsize, NumberOfProbePackets, BoolDropped\n";
		}

		long int sTime = (packet->getSendingTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);

		outputFile << sTime << ",";
		outputFile << packet->getSequenceNumber() << ",";
		outputFile << packet->getPacketNumber() << ",";
		outputFile << packet->getPayloadSize() << ",";
		outputFile << packet->getNumberOfProbePackets() << "\n";
	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Logs all informations from a MyPacket
 * @param path for the file
 * @param packet, which should be logged
 */
void LogFileWriter::writeLogfileForPacketsSendQueue(std::string path, MyPacket* packet) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all TStamps are ns\n";
			outputFile << "#a dropped packet implys recTstamp = sendTstamp\n";
			outputFile
					<< "#RecTstamp, SendTstamp, Delay, SeqNr, PacketNr, Payloadsize, NumberOfProbePackets, BoolDropped\n";
		}

		long int aTime = (packet->getArrivalTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);
		long int sTime = (packet->getSendingTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);
		long int dTime = ((packet->getArrivalTimeForLogging() - packet->getSendingTimeForLogging()).dbl() *  kMULTIPLIER_S_TO_NS);

		outputFile << aTime << ",";
		outputFile << sTime << ",";
		outputFile << dTime << ",";
		outputFile << packet->getSequenceNumber() << ",";
		outputFile << packet->getPacketNumber() << ",";
		outputFile << packet->getPayloadSize() << ",";
		outputFile << packet->getNumberOfProbePackets() << ",";
		outputFile << (packet->getDroppedSimulated() ? "true" : "false") << "\n";
	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Logs all informations from a SendHeadAndDeletePacket
 * @param path for the file
 * @param packet, which should be logged
 */
void LogFileWriter::writeLogFileForPacketsSendHead(std::string path, SendHeadAndDeletePacket* sendHead) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all TStamps are ns\n";
			outputFile << "#SendTstamp, RecTstamp, Delay DelPacket\n";
		}

		long int aTime = (sendHead->getArrivalTimeForTesting().dbl() *  kMULTIPLIER_S_TO_NS);
		long int sTime = (sendHead->getSendingTimeForTesting().dbl() *  kMULTIPLIER_S_TO_NS);
		long int dTime = ((sendHead->getSendingTimeForTesting() - sendHead->getArrivalTimeForTesting()).dbl() *  kMULTIPLIER_S_TO_NS);

		outputFile << aTime << ",";
		outputFile << sTime << ",";
		outputFile << dTime << ",";
		outputFile << sendHead->getDeletePacketWithName() << "\n";
	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Logs all informations from a MyPacket
 * @param path for the file
 * @param packet, which should be logged
 */
void LogFileWriter::writeLogfileForPacketsReceivedQueue(std::string path, MyPacket* packet) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all TStamps are ns\n";
			outputFile << "#a dropped packet implys recTstamp = sendTstamp\n";
			outputFile << "#RecTstamp, SeqNr, PacketNr, Payloadsize, NumberOfProbePackets, BoolDropped\n";
		}

		long int aTime = (packet->getArrivalTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);

		outputFile << aTime << ",";
		outputFile << packet->getSequenceNumber() << ",";
		outputFile << packet->getPacketNumber() << ",";
		outputFile << packet->getPayloadSize() << ",";
		outputFile << packet->getNumberOfProbePackets() << ",";
		outputFile << (packet->getDroppedSimulated() ? "true" : "false") << "\n";
	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Logs all informations from a BackboneDelay for a MyPacket
 * @param path for the file
 * @param packet, which should be logged
 * @param bbdelay, current backbone delay
 * @param currentBbdelay
 */
void LogFileWriter::writeLogFileForPacketsBbdelay(std::string path, MyPacket* packet, Bbdelay* bbdelay,
		simtime_t currentBbdelay) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all Tstamps are ns\n";
			outputFile << "#a dropped packet implys recTstamp = sendTstamp\n";
			outputFile
					<< "#RecTstamp, SeqNr, PacketNr, NumberOfProbePackets, Payloadsize, bbdelay, valid, currentBbdelay\n";
		}

		long int aTime = (packet->getArrivalTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);

		outputFile << aTime << ",";
		outputFile << packet->getSequenceNumber() << ",";
		outputFile << packet->getPacketNumber() << ",";
		outputFile << packet->getNumberOfProbePackets() << ",";
		outputFile << packet->getPayloadSize() << ",";
		outputFile << bbdelay->bbdelay() << ",";
		outputFile << (bbdelay->validbbdelay() ? "true" : "false") << ",";
		outputFile << currentBbdelay << "\n";
	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Logs all informations from a Datarate for a MyPacket
 * @param path for the file
 * @param packet, which should be logged
 * @param datarate, current datarate
 * @param currentDatarateDelay
 */
void LogFileWriter::writeLogFileForPacketsDatarate(std::string path, MyPacket* packet, Datarate* datarate,
		simtime_t currentDatarateDelay) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all Tstamps are ns\n";
			outputFile << "#a dropped packet implys recTstamp = sendTstamp\n";
			outputFile << "#RecTstamp, SeqNr, PacketNr, Payloadsize, NumberOfProbePackets, datarate, droprate, valid\n";
		}

		long int aTime = (packet->getArrivalTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);
		outputFile << aTime << ",";
		outputFile << packet->getSequenceNumber() << ",";
		outputFile << packet->getPacketNumber() << ",";
		outputFile << packet->getNumberOfProbePackets() << ",";
		outputFile << packet->getPayloadSize() << ",";
		outputFile << datarate->datarate() << ",";
		outputFile << datarate->droprate() << ",";
		outputFile << (datarate->validdatarate() ? "true" : "false") << ",";
		outputFile << currentDatarateDelay << "\n";
	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Logs all stored timestamp from a MyPacket
 * @param path for the file
 * @param packet, which should be logged
 */
void LogFileWriter::writeLogfileForPacketsTimestamps(std::string path, MyPacket* packet) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all TStamps are ns\n";
			outputFile << "#a dropped packet implys recTstamp = sendTstamp\n";
			outputFile << "#SeqNr, PacketNr, NumberOfProbePackets, SequenceNumber, Timestamps[0..n], BoolDropped\n";
		}

		for (unsigned int i = 0; i < packet->getTimestampsArraySize(); i++) {
			outputFile << packet->getTimestamps(i) << ",";
		}
		outputFile << (packet->getDroppedSimulated() ? "true " : "false") << "\n";
	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}


/**
 * Logs everyhing
 * @param path for the file
 * @param packet, which should be logged
 */
void LogFileWriter::writeLogfileForPacketsChar(std::string path, MyPacket* packet) {

	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all TStamps are ns\n";
			outputFile << "#NormTstamp, Latitude, Longitude, Data Rate, Data Rate Sim, BoolValidDr, BackBoneDelay, ";
			outputFile << "BackBoneDelaySim, BoolValidBbDelay, Delay, Droprate, SeqNr, ModemMode, Trainsize, Packetsize\n";
			outputFile << "#CURRENTLY NOT IN USE\n";
		}

		long int aTime = packet->getStartTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS;
		long int bbTime = packet->getBbdelay().bbdelay().dbl() * kMULTIPLIER_S_TO_NS;
		long int bbsTime = packet->getBbdelay().bbdelay().dbl() * kMULTIPLIER_S_TO_NS;

		long int dtime = 0;
		std::size_t found = path.find(kSTRING_FILE_UPLOADCHAR);
		if (found!=std::string::npos){
			dtime = (packet->getSCDSQ() - packet->getDCDSQ()).dbl();
		} else {
			dtime = (packet->getSCDRQ() - packet->getDCDRQ()).dbl();
		}

		outputFile << aTime << ",";
		outputFile << packet->getDatarate().latitude() << ",";
		outputFile << packet->getDatarate().longitude() << ",";
		outputFile << packet->getDatarate().datarate()/kDIVIDER_KB_TO_B << ",";
		outputFile << packet->getDatarate().datarateSim()/kDIVIDER_KB_TO_B << ",";
		outputFile << (packet->getDatarate().validdatarate() ? "true" : "false")<< ",";
		outputFile << bbTime << ",";
		outputFile << bbsTime << ",";
		outputFile << (packet->getBbdelay().validbbdelay() ? "true" : "false")<< ",";
		outputFile << dtime << ",";
		outputFile << packet->getDatarate().droprate() << ",";
		outputFile << packet->getSequenceNumber() << ",";
		outputFile << packet->getDatarate().modemMode() << ",";
		outputFile << packet->getNumberOfProbePackets() << ",";
		outputFile << packet->getPayloadSize();
		outputFile << "\n";

	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Logs everyhing
 * @param path for the file
 * @param packet, which should be logged
 */
void LogFileWriter::writeLogfileForPacketsComplete(std::string path, MyPacket* packet) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#all TStamps are ns\n";
			outputFile << "#sendTstamp , recTstampMeasured , recTstampSimulated , droppedMeasured , ";
			outputFile << "droppedSimulation , delayMeasured[ns] , delaySimulated[ns], sourceIP:Port , ";
			outputFile << "destinationIP:Port , payLoadSize , seqnr , packetnr , trainsize , ";
			outputFile << "dCRSQ , dDLSQ , dCDRQ , dCRRQ , sCRSQ, sCDSQ, sCDRQ, sCRRQ, tstamp.size , tstampList (...)\n";
		}

		long int aTime = (packet->getArrivalTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);
		long int rTime = (packet->getRecTstampMeasured().dbl() *  kMULTIPLIER_S_TO_NS);
		long int sTime = (packet->getStartTimeForLogging().dbl() *  kMULTIPLIER_S_TO_NS);
		long int dTime = ((packet->getArrivalTimeForLogging() - packet->getStartTimeForLogging()).dbl() *  kMULTIPLIER_S_TO_NS);
		long int dmTime = (packet->getDelayMeasured().dbl() *  kMULTIPLIER_S_TO_NS);

		long int dtime1 = packet->getDCRSQ().dbl() *  kMULTIPLIER_S_TO_NS;
		long int dtime2 = packet->getDCDSQ().dbl() *  kMULTIPLIER_S_TO_NS;
		long int dtime3 = packet->getDCDRQ().dbl() *  kMULTIPLIER_S_TO_NS;
		long int dtime4 = packet->getDCRRQ().dbl() *  kMULTIPLIER_S_TO_NS;
		long int stime1 = packet->getSCRSQ().dbl() *  kMULTIPLIER_S_TO_NS;
		long int stime2 = packet->getSCDSQ().dbl() *  kMULTIPLIER_S_TO_NS;
		long int stime3 = packet->getSCDRQ().dbl() *  kMULTIPLIER_S_TO_NS;
		long int stime4 = packet->getSCRRQ().dbl() *  kMULTIPLIER_S_TO_NS;

		outputFile << sTime << ",";
		outputFile << rTime << ",";
		outputFile << aTime << ",";
		outputFile << (packet->getDroppedMeasured() ? "true" : "false") << ",";

		outputFile << (packet->getDroppedSimulated() ? "true " : "false") << ",";
		outputFile << dmTime << ",";
		outputFile << dTime << ",";
		outputFile << packet->getSourceIP() << ":";

		outputFile << packet->getSourcePort() << ",";
		outputFile << packet->getDestinationIP() << ":";
		outputFile << packet->getDestinationPort() << ",";
		outputFile << packet->getPayloadSize() << ",";
		outputFile << packet->getSequenceNumber() << ",";
		outputFile << packet->getPacketNumber() << ",";
		outputFile << packet->getNumberOfProbePackets() << ",";

		outputFile << dtime1 << ",";
		outputFile << dtime2 << ",";
		outputFile << dtime3 << ",";
		outputFile << dtime4 << ",";
		outputFile << stime1 << ",";
		outputFile << stime2 << ",";
		outputFile << stime3 << ",";
		outputFile << stime4 << ",";
		outputFile << packet->getTimestampsArraySize() << ",";
		for (unsigned int i = 0; i < packet->getTimestampsArraySize()-1; i++) {
			outputFile << packet->getTimestamps(i) << ",";
		}
		outputFile << packet->getTimestamps(packet->getTimestampsArraySize()-1);
		outputFile << "\n";

	} else {
		EV<< "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Logs all stored timestamp from a MyPacket
 * @param path for the file
 * @param packet, which should be logged
 */
void LogFileWriter::writeLogfileForPacketsStats(std::string path, std::string direction, int counter[]) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#Direction , Send , ReceivedReal , ReceivedSim , DroppedReal , DroppedSim\n";
		}

		outputFile << direction << ",";
		for (int i=0; i<4; i++){
			outputFile << counter[i] << ",";
		}
		outputFile << counter[4] << "\n";

	} else {
		EV << "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}


/**
 *
 * @param path
 * @param packet
 */
void LogFileWriter::writeLogfileForPacketsStatsDiff(std::string path, MyPacket *packet) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			std::size_t found1 = path.find(kSTRING_FILE_UPLOADSTATS_FIRSTDIFF);
			std::size_t found2 = path.find(kSTRING_FILE_DOWNLOADSTATS_FIRSTDIFF);
			if (found1!=std::string::npos || found2!=std::string::npos){
				outputFile << "#Difference [ms] between delays of the first packets measured and first packets simulated per train\n";
			} else {
				outputFile << "#Difference [ms] between delays of the last packets measured and first last simulated per train\n";
			}
		}

		long int dtime = (packet->getRecTstampMeasured() - packet->getArrivalTime()).dbl() * kMULTIPLIER_S_TO_MS;

		outputFile << dtime << "\n";

	} else {
		EV << "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}

/**
 * Writes output, so that CDF could be used later
 * @param path
 * @param packet
 */
void LogFileWriter::writeLogfileForPacketsCDF(std::string path, MyPacket *packet) {
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		std::size_t found1 = path.find(kSTRING_FILE_UPLOAD_SIMCDF);
		std::size_t found2 = path.find(kSTRING_FILE_UPLOAD_REALCDF);
		std::size_t found3 = path.find(kSTRING_FILE_DOWNLOAD_SIMCDF);
		std::size_t found4 = path.find(kSTRING_FILE_DOWNLOAD_REALCDF);

		if (is_textfile_empty(path)) {
			if (found1!=std::string::npos){			outputFile << "#CDF for simulated upload delay\n";
			} else if (found2!=std::string::npos){	outputFile << "#CDF for measured upload delay\n";
			} else if (found3!=std::string::npos){	outputFile << "#CDF for simulated download delay\n";
			} else if (found4!=std::string::npos){	outputFile << "#CDF for measured download delay\n";
			} else {								outputFile << "#error header\n";
			}
		}

		long int dtime = 0;

		// simulated or measured
		if (found1!=std::string::npos || found3!=std::string::npos){
			dtime = (packet->getArrivalTime() - packet->getStartTimeForLogging()).dbl() * kMULTIPLIER_S_TO_MS;
		} else {
			dtime = (packet->getRecTstampMeasured() - packet->getStartTimeForLogging()).dbl() * kMULTIPLIER_S_TO_MS;
		}

		outputFile << dtime << "\n";


	} else {
		EV << "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();
}



/**
 *
 * @param path
 * @param packet
 * @param direction
 * @param counter
 */
void LogFileWriter::writeLogfileForTrainsStats(std::string path, MyPacket* packet, std::string direction, double counter[]){
	std::ofstream outputFile;
	outputFile.open(path.c_str(), std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		if (is_textfile_empty(path)) {
			outputFile << "#All timestamps are in ms\n";
			outputFile << "#no data available is represented by NaN\n";
			outputFile << "#trainstart,seqnr,delayfirstreal,delaylastreal,delayfirstsim,delaylastsim,";
			outputFile << "trainsize,dropsreal,dropssim,payloadsize,dropsumreal,dropsumsim,";
			if (strcasecmp(kSTRING_UPLOAD, direction.c_str()) == 0){
				outputFile << "sCRSQ\n";
			} else {
				outputFile << "sCDRQ\n";
			}
		}

		long tstamp = packet->getStartTimeForLogging().dbl() * kMULTIPLIER_S_TO_MS;
		long delayfirstreal = counter[0] * kMULTIPLIER_S_TO_MS;
		long delaylastreal = counter[1] * kMULTIPLIER_S_TO_MS;
		long delayfirstsim = counter[2] * kMULTIPLIER_S_TO_MS;
		long delaylastsim = counter[3] * kMULTIPLIER_S_TO_MS;
		long stime = 0;

		outputFile << tstamp << ",";
		outputFile << packet->getSequenceNumber() << ",";
//		outputFile << delayfirstreal << ",";
//		outputFile << delaylastreal << ",";
		(counter[0] == 0) ? outputFile << "NaN," : outputFile << delayfirstreal << ",";
		(counter[1] == 0) ? outputFile << "NaN," : outputFile << delaylastreal << ",";
//		outputFile << delayfirstsim << ",";
//		outputFile << delaylastsim << ",";
		(counter[2] == 0) ? outputFile << "NaN," : outputFile << delayfirstsim << ",";
		(counter[3] == 0) ? outputFile << "NaN," : outputFile << delaylastsim << ",";
		outputFile << packet->getNumberOfProbePackets() << ",";
		outputFile << (int)counter[4] << ",";
		outputFile << (int)counter[5] <<  ",";
		outputFile << packet->getPayloadSize() << ",";
		outputFile << (int)counter[6] <<  ",";
		outputFile << (int)counter[7] <<  ",";
		if (strcasecmp(kSTRING_UPLOAD, direction.c_str()) == 0){
			stime = (packet->getSCRSQ().dbl() * kMULTIPLIER_S_TO_NS);
		} else {
			stime = (packet->getSCDRQ().dbl() * kMULTIPLIER_S_TO_NS);
		}
		outputFile << stime << "\n";

	} else {
		EV << "\nLogwriter could not open " << path << endl << endl;
	}

	//outputFile.flush();
	outputFile.close();

}

} /* namespace projekt */

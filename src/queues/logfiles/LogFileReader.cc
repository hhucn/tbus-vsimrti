/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "LogFileReader.h"

namespace projekt {

/**
 * default constructor
 */
LogFileReader::LogFileReader() {
	kTRUE_LOWERCASE = "true";
}

/**
 * default destructor
 */
LogFileReader::~LogFileReader() {
}

/**
 * Parses all Datarates in given path. Format should be:
 * NormTstamp, Latitude, Longitude, Data Rate, Data Rate Sim, BoolValidDr, Droprate, SeqNr, ModemMode, Trainsize, Packetsize
 * @param path: given input path
 * @param delimiter: used in file
 * @param logdata[]: return array
 * @param count: count of datarates, which will be parsed (be aware of max[eof,count])
 */
std::vector<LogDatarate*> LogFileReader::parseDatarate(std::string path, std::string delimiter, std::vector<LogDatarate*> logData, long limit) {
	long count = 0;
	double maxSimtime = simTime().getMaxTime().dbl();
	std::ifstream fin;
	bool shouldBeAdded = false;
	bool v;

	fin.open(path.c_str());
	if (!fin.good()){
		logData.clear();
		return logData;
	}

	// read each line of the file
	while (!fin.eof() && count < limit) {

		// init buffer and tokens
		char buf[MAX_CHARS_PER_LINE];
		fin.getline(buf, MAX_CHARS_PER_LINE);
		int n = 0;
		const char* token[MAX_TOKENS_PER_LINE] = { };
		token[0] = strtok(buf, delimiter.c_str());

		// read all tokens
		if (token[0]) {
			for (n = 1; n < MAX_TOKENS_PER_LINE; n++) {
				token[n] = strtok(0, delimiter.c_str());
				if (!token[n])
					break;
			}
		}

		// store and log tokens
		LogDatarate* data = new LogDatarate();

		if (token[0] && ((strstr(token[0], "#") - token[0]) != 0)) {
			for (int i = 0; i < n; i++) {

				// catch values, which are out of range and replace with minimal delay
				// so we got no nullpointers while simulating :)
				if (i == 0 && (atof(token[i]) < 0 || (atof(token[i]) / kMULTIPLIER_S_TO_NS) > maxSimtime)) {
					int index = logData.size() - 1;
					std::stringstream ss;
					ss << logData.at(index)->normTstamp();
					token[0] = ss.str().c_str();
				}

				switch (i) {
					case 0:
						data->setNormTstamp(atof(token[i]) / kMULTIPLIER_S_TO_NS);
						break;
					case 1:
						data->setLatitude(atof(token[i]));
						break;
					case 2:
						data->setLongitude(atof(token[i]));
						break;
					case 3:
						data->setDatarate(atof(token[i]) * kDIVIDER_KB_TO_B);
						break;
					case 4:
						data->setDatarateSim(atof(token[i]) * kDIVIDER_KB_TO_B);
						break;
					case 5:
						v = strcasecmp(kTRUE_LOWERCASE, token[i]) == 0 ? true : false;
						data->setValid(v);
						break;
					case 6:
						data->setDroprate(atof(token[i]));
						break;
					case 7:
						data->setSeqNr(atoi(token[i]));
						break;
					case 8:
						data->setModemMode(token[i]);
						break;
					case 9:
						data->setTrainsize(atoi(token[i]));
						break;
					case 10:
						data->setPacketsize(atoi(token[i]));
						break;
				}
			}
			shouldBeAdded = true;
		} else {
			shouldBeAdded = false;
		}

		if (shouldBeAdded) {
			count++;
			logData.push_back(data);
		}
	}
	return logData;
}

/**
 * Parses all BBdelays in given path. Format should be:
 * NormTstamp, Latitude, Longitude, BackBoneDelay, BackBoneDelaySim, BoolValidBbDelay, Delay, Droprate, SeqNr, ModemMode, Trainsize, Packetsize
 * @param path: given input path
 * @param delimiter: used in file
 * @param logBbdelay[]: return array
 * @param count: count of datarates, which will be parsed (be aware of max[eof,count])
 */
std::vector<LogBbdelay*> LogFileReader::parseBbdelay(std::string path, std::string delimiter, std::vector<LogBbdelay*> logBbdelay, long limit) {
	long count = 0;
	double maxSimtime = simTime().getMaxTime().dbl();
	std::ifstream fin;
	bool v;
	bool shouldBeAdded = false;

	fin.open(path.c_str());
	if (!fin.good()){
		logBbdelay.clear();
		return logBbdelay;
	}

	// read each line of the file
	while (!fin.eof() && count < limit) {

		// init buffer and tokens
		char buf[MAX_CHARS_PER_LINE];
		fin.getline(buf, MAX_CHARS_PER_LINE);
		int n = 0;
		const char* token[MAX_TOKENS_PER_LINE] = { };
		token[0] = strtok(buf, delimiter.c_str());

		// read all tokens
		if (token[0]) {
			for (n = 1; n < MAX_TOKENS_PER_LINE; n++) {
				token[n] = strtok(0, delimiter.c_str());
				if (!token[n])
					break;
			}
		}

		// store and log tokens
		LogBbdelay* bbdelay = new LogBbdelay();

		if (token[0] && ((strstr(token[0], "#") - token[0]) != 0)) {
			for (int i = 0; i < n; i++) {

				// catch values, which are out of range and replace with minimal delay
				// so we got no nullpointers while simulating :)
				if (i == 3 && (atof(token[i]) < 0 || (atof(token[i]) / kMULTIPLIER_S_TO_NS) > maxSimtime)) {
					int index = logBbdelay.size() - 1;
					std::stringstream ss;
					ss << logBbdelay.at(index)->backboneDelay();
					token[3] = ss.str().c_str();
				}
				if (i == 4 && (atof(token[i]) < 0 || (atof(token[i]) / kMULTIPLIER_S_TO_NS) > maxSimtime)) {
					int index = logBbdelay.size() - 1;
					std::stringstream ss;
					ss << logBbdelay.at(index)->backboneDelay();
					token[4] = ss.str().c_str();
				}

				switch (i) {
					case 0:
						bbdelay->setNormTstamp(atof(token[i]) / kMULTIPLIER_S_TO_NS);
						break;
					case 1:
						bbdelay->setLatitude(atof(token[i]));
						break;
					case 2:
						bbdelay->setLongitude(atof(token[i]));
						break;
					case 3:
						bbdelay->setBackboneDelay(atof(token[i]) / kMULTIPLIER_S_TO_NS);
						break;
					case 4:
						bbdelay->setBackboneDelaySim(atof(token[i]) / kMULTIPLIER_S_TO_NS);
						break;
					case 5:
						v = strcasecmp(kTRUE_LOWERCASE, token[i]) == 0 ? true : false;
						bbdelay->setValid(v);
						break;
					case 6:
						bbdelay->setDelay(atof(token[i]));
						break;
					case 7:
						bbdelay->setDroprate(atof(token[i]));
						break;
					case 8:
						bbdelay->setSeqNr(atoi(token[i]));
						break;
					case 9:
						bbdelay->setModemMode(token[i]);
						break;
					case 10:
						bbdelay->setTrainsize(atoi(token[i]));
						break;
					case 11:
						bbdelay->setPacketsize(atoi(token[i]));
						break;
				}
			}
			shouldBeAdded = true;
		} else {
			shouldBeAdded = false;
		}

		if (shouldBeAdded) {
			count++;
			logBbdelay.push_back(bbdelay);
		}
	}
	return logBbdelay;
}

/**
 * Parses all Packets in given path. Format should be:
 * NormSendTstamp, NormRecTstamp, SeqNr, PacketNr, NumberOfProbePackets, Payloadsize, BoolDropped
 * @param path: given input path
 * @param delimiter: used in file
 * @param logPackets[]: return array
 * @param count: count of datarates, which will be parsed (be aware of max[eof,count])
 */
std::vector<LogPackets*> LogFileReader::parsePackets(std::string path, std::string delimiter, std::vector<LogPackets*> logPackets, long limit) {
	long count = 0;
	double maxSimtime = simTime().getMaxTime().dbl();
	std::ifstream fin;
	bool v;
	bool shouldBeAdded = false;

	fin.open(path.c_str());
	if (!fin.good()){
		logPackets.clear();
		return logPackets;
	}

	// read each line of the file
	while (!fin.eof() && count < limit) {

		// init buffer and tokens
		char buf[MAX_CHARS_PER_LINE];
		fin.getline(buf, MAX_CHARS_PER_LINE);
		int n = 0;
		const char* token[MAX_TOKENS_PER_LINE] = { };
		token[0] = strtok(buf, delimiter.c_str());

		// read all tokens
		if (token[0]) {
			for (n = 1; n < MAX_TOKENS_PER_LINE; n++) {
				token[n] = strtok(0, delimiter.c_str());
				if (!token[n])
					break;
			}
		}

		// store and log tokens
		LogPackets* packet = new LogPackets();

		if (token[0] && ((strstr(token[0], "#") - token[0]) != 0)) {
			for (int i = 0; i < n; i++) {

				// catch values, which are out of range and replace with minimal delay
				// so we got no nullpointers while simulating :)
				if (i == 0 && (atof(token[i]) < 0 || (atof(token[i]) / kMULTIPLIER_S_TO_NS) > maxSimtime)) {
					int index = logPackets.size() - 1;
					std::stringstream ss;
					ss << logPackets.at(index)->normSendTstamp();
					token[0] = ss.str().c_str();
				}
				if (i == 1 && (atof(token[i]) < 0 || (atof(token[i]) / kMULTIPLIER_S_TO_NS) > maxSimtime)) {
					int index = logPackets.size() - 1;
					std::stringstream ss;
					ss << logPackets.at(index)->normSendTstamp();
					token[i] = ss.str().c_str();
				}
				if (i == 5 && (atol(token[i]) < 0 || (atof(token[i]) / kMULTIPLIER_S_TO_NS) > maxSimtime)) {
					int index = logPackets.size() - 1;
					std::stringstream ss;
					ss << logPackets.at(index)->normSendTstamp();
					token[i] = ss.str().c_str();
				}

				switch (i) {
					case 0:
						packet->setNormSendTstamp(atof(token[i]) / kMULTIPLIER_S_TO_NS);
						break;
					case 1:
						packet->setNormRecTstamp(atof(token[i]) / kMULTIPLIER_S_TO_NS);
						break;
					case 2:
						packet->setSeqNr(atoi(token[i]));
						break;
					case 3:
						packet->setPacketNr(atoi(token[i]));
						break;
					case 4:
						packet->setNumberOfProbePackets(atoi(token[i]));
						break;
					case 5:
						packet->setPayloadsize(atol(token[i]));
						break;
					case 6:
						v = strcasecmp(kTRUE_LOWERCASE, token[i]) == 0 ? true : false;
						packet->setBoolDroppedMeasured(v);
						break;
				}
			}
			shouldBeAdded = true;
		} else {
			shouldBeAdded = false;
		}

		if (shouldBeAdded) {
			count++;
			logPackets.push_back(packet);
		}
	}
	return logPackets;
}

} /* namespace projekt */

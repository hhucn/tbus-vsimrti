/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef LOGFILEWRITER_H_
#define LOGFILEWRITER_H_

#include "Bbdelay.h"
#include "Datarate.h"
#include "MyPacket_m.h"
#include "Localtime.h"
#include "Preamble.h"
#include "SendHeadAndDeletePacket_m.h"

#include <omnetpp.h>
#include <iostream>
#include <fstream>
#include <string.h>

namespace projekt {

	class LogFileWriter {
		private:
			virtual bool is_textfile_empty(std::string filename);

			//output path for logfiles
			std::string outputPath;
			std::string localTime;

		public:
			LogFileWriter();
			virtual ~LogFileWriter();

			virtual void writeLogToHarddrive(MyPacket* job, std::string folder, std::string filename, Bbdelay* currentBbdelay, Datarate* currentDatarate);

			virtual void writeLogfileForBbdelay(std::string path, Bbdelay* bbdelay);
			virtual void writeLogfileForDatarate(std::string path, Datarate* datarate);
			virtual void writeLogfileForPackets(std::string path, MyPacket* packet);
			virtual void writeLogfileForPacketsReceived(std::string path, MyPacket* packet);
			virtual void writeLogfileForPacketsSend(std::string path, MyPacket* packet);
			virtual void writeLogfileForPacketsSendQueue(std::string path, MyPacket* packet);
			virtual void writeLogfileForPacketsReceivedQueue(std::string path, MyPacket* packet);
			virtual void writeLogFileForPacketsSendHead(std::string path, SendHeadAndDeletePacket* sendHead);
			virtual void writeLogFileForPacketsBbdelay(std::string path, MyPacket* packet, Bbdelay* bbdelay, simtime_t currentBackboneDelay);
			virtual void writeLogFileForPacketsDatarate(std::string path, MyPacket* packet, Datarate* datarate, simtime_t currentDatarateDelay);
			virtual void writeLogfileForPacketsTimestamps(std::string path, MyPacket* packet);
			virtual void writeLogfileForPacketsCDF(std::string path, MyPacket *packet);
			virtual void writeLogfileForPacketsChar(std::string path, MyPacket* packet);
			virtual void writeLogfileForPacketsComplete(std::string path, MyPacket* packet);
			virtual void writeLogfileForPacketsStats(std::string path, std::string direction, int counter[]);
			virtual void writeLogfileForPacketsStatsDiff(std::string path, MyPacket* packet);
			virtual void writeLogfileForTrainsStats(std::string path, MyPacket* packet, std::string direction, double counter[]);
	};

} /* namespace projekt */
#endif /* LOGFILEWRITER_H_ */

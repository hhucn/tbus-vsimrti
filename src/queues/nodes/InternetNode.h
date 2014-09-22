/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef INTERNETNODE_H_
#define INTERNETNODE_H_

#include <omnetpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <math.h>
#include "MyPacket_m.h"
#include "Preamble.h"
#include "LogFileWriter.h"
#include "LogFileReader.h"
#include "LogCDF.h"

namespace projekt {

	class InternetNode: public cSimpleModule{

		private:
			bool isInTestMode;
			bool firstNonDroppedSimulatedPacket;
			bool firstNonDroppedMeasuredPacket;

            std::string source_ip;
            std::string source_port;

			// last non dropped packets in current train
            MyPacket* lastInTrainNonDroppedSimulatedPacket;
            MyPacket* lastInTrainNonDroppedMeasuredPacket;
            MyPacket* firstInTrainNonDroppedSimulatedPacket;
            MyPacket* firstInTrainNonDroppedMeasuredPacket;


			// signals for logging in omnet
			simsignal_t signalDelay, signalDropMeasured, signalDropSimulated;

			//counter for statistic
			int countPacketSend;
			int countUploadReceivedReal;
			int countUploadReceivedSim;
			int countUploadSumDroppedReal;
			int countUploadSumDroppedSim;
			int countUploadDroppedReal;
			int countUploadDroppedSim;

			//times for logging
			simtime_t delayfirstreal;
			simtime_t delaylastreal;
			simtime_t delayfirstsim;
			simtime_t delaylastsim;

			//output path for logfiles
			std::string outputPath;
			std::string localTime;

			//Filewrite for logfiles
			LogFileWriter *logwriter;

			virtual void sendLogFiles();
			virtual void writeLogToHarddrive(MyPacket *job, std::string filename);
			virtual std::string buildAndMakePath(std::string filename);

		public:
			InternetNode();
			InternetNode(bool testMode);
			virtual ~InternetNode();

		protected:
			virtual void initialize();
			virtual void finish();
			virtual void handleMessage(cMessage* msg);
			virtual void sendMessage(MyPacket* job, simtime_t delayUntilNow);
	};

} /* namespace projekt */
#endif /* INTERNETNODE_H_ */

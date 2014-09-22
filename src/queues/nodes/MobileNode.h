/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef NODE_H_
#define NODE_H_

#include <omnetpp.h>
#include <math.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include "MyPacket_m.h"
#include "Preamble.h"
#include "CRRQ.h"
#include "CRSQ.h"
#include "CDSQ.h"
#include "CDRQ.h"
#include "LogFileWriter.h"
#include "LogFileReader.h"
#include "LogCDF.h"
#include "InternetNode.h"

namespace projekt {

	class MobileNode: public cSimpleModule{

		private:
			// boolean for test mode
			bool isInTestMode;
			bool firstNonDroppedSimulatedPacket;
			bool firstNonDroppedMeasuredPacket;

			// some counter
			int sequenceNumber;
			int clientCount;
			int controlPacketsSequenceNumber;

			// last non dropped packets in current train
            MyPacket* lastInTrainNonDroppedSimulatedPacket;
            MyPacket* lastInTrainNonDroppedMeasuredPacket;
            MyPacket* firstInTrainNonDroppedSimulatedPacket;
            MyPacket* firstInTrainNonDroppedMeasuredPacket;

			// count for statistic
			int countMsgSend;
			int countPacketSend;
			int countDownloadReceivedReal;
			int countDownloadReceivedSim;
			int countDownloadSumDroppedReal;
			int countDownloadSumDroppedSim;
			int countDownloadDroppedReal;
			int countDownloadDroppedSim;

			//times for logging
			simtime_t delayfirstreal;
			simtime_t delaylastreal;
			simtime_t delayfirstsim;
			simtime_t delaylastsim;

			// output path for logfiles
			std::string outputPath;
			std::string localTime;

			// filewrite for logfiles
			LogFileWriter *logwriter;

			std::vector <std::string> vectorAllNodeIPs;
			double trainSendIntervall;
			std::string source_ip;
			std::string source_port;

			// emit signals
			simsignal_t signalDelay, signalSendingTime, signalArrivalTime, signalTrainLength, signalDropMeasured, signalDropSimulated;

			cMessage *selfMsgStartTrain, *selfMsgChangeDelays;

			virtual void sendLogFiles();
			virtual void writeLogToHarddrive(MyPacket *job, std::string filename);
			virtual std::string buildAndMakePath(std::string filename);

			// sending control messages, when some values had changed
			virtual void setUpDatarate(Datarate* newDatarate);
			virtual void setDownDatarate(Datarate* newDatarate);
			virtual void setUpBbdelay(Bbdelay* newBbdelay);
			virtual void setDownBbdelay(Bbdelay* newBbdelay);

		public:
			MobileNode();
			MobileNode(bool testMode);
			virtual ~MobileNode();

		protected:
			virtual void initialize();
			virtual void finish();

			virtual void sendInitialMessage();
			virtual void showInitialMsgBox();

			virtual void handleMessage(cMessage* msg);
			virtual void handleSelfMessage(cMessage* msg);

			virtual void sendPacketTrain();
			virtual void sendMessage(MyPacket* job, simtime_t delayUntilNow);
	};
}
;

#endif /* NODE_H_ */

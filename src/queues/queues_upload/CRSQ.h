/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef CRSQ_H_
#define CRSQ_H_

#include "MyPacket_m.h"
#include "SendHeadAndDeletePacket_m.h"
#include "Preamble.h"
#include "Localtime.h"
#include "LogFileWriter.h"
#include "IDatarateQueue.h"
#include "IQueue.h"

#include <omnetpp.h>
#include <Datarate.h>

namespace projekt {

class CRSQ : public cSimpleModule, public IQueue, public IDatarateQueue  {

	private:
		// signals
		simsignal_t signalDatarate, signalLossprob, signalLossprobRnd, signalPacketDatarateDelay, signalDropMeasured, signalDropSimulated;

		// remaining buffer in queue
		int freeBufferInByte;

		// boolean for test mode (in test mode we dont want logs)
		bool isInTestMode;

		//output path for logfiles
		std::string outputPath;

		//Filewrite for logfiles
		LogFileWriter *logwriter;

		// local time for output path
		std::string localTime;

		// our packet queue
		cPacketQueue *queue;

		// current self message for dispatching head of queue
		SendHeadAndDeletePacket *currSendHeadCRSQPacket;

		//some initialization value to overcome null pointer exceptions that happen at first dataratechange event
		Datarate* currentDatarate;

		//store all datarate characteristics since the last dispatch
		std::vector<Datarate*> datarateVector;

		//and store the timestamps (in nanoseconds) of the characteristics
		std::vector<simtime_t> drTimeVector;

		//how many bytes went over the air since the last dispatch
		double capacityBytes;

		//current loss probability
		double currentLossProbability;

		// current latency
		simtime_t currentLatency;

	public:
		CRSQ();
		CRSQ(bool testModeEnabled);
		virtual ~CRSQ();
		virtual void datarateChanged(Datarate* newDatarate, simtime_t currentSimtime);
		friend class CRSQ_test; // need for tests

	protected:
		virtual void initialize();
		virtual void writeLogToHarddrive(MyPacket* job, std::string filename, simtime_t currentDelay);

		virtual void handleMessage(cMessage* msg);
		virtual void handleSendHeadAndDeletePacket(simtime_t currentSimtime, SendHeadAndDeletePacket* sendHeadPacket);
		virtual void handleControlMessage(cMessage* msg);

		virtual void scheduleNewSendHeadAndDeletePacket(simtime_t currentSimtime);
		virtual void scheduleNewSendHeadAndDeletePacket(simtime_t currentSimtime, simtime_t delay);
		virtual void manipulateSelfMessageProcess(simtime_t currentSimtime);

		virtual void addPacketToQueue(MyPacket* job, simtime_t currentSimtime);

		virtual double calculateLossProbability(simtime_t currentSimtime);
		virtual simtime_t calculateDatarateDelay(double packetsize, simtime_t currentSimtime);

		virtual void transmitOrLooseHead(simtime_t currentSimtime, bool retBool[]);
		virtual void dispatch(simtime_t currentSimtime, bool retBool[]);
}
;
}
;

#endif /* CRSQ_H_ */

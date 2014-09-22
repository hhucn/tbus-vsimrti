/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef CDSQ_H_
#define CDSQ_H_

#include "SendHeadAndDeletePacket_m.h"
#include "Preamble.h"
#include "Bbdelay.h"
#include "Localtime.h"
#include "LogFileWriter.h"
#include "IBbdelayQueue.h"
#include "IQueue.h"

#include <omnetpp.h>
#include <string.h>

namespace projekt {

class CDSQ : public cSimpleModule, public IQueue, public IBbdelayQueue {

	private:
		// signals
		simsignal_t signalBbdelay;

		// our packet queue
		cPacketQueue* queue;

		// current self message for dispatching head of queue
		SendHeadAndDeletePacket *currSendHeadCDSQPacket;

		// boolean for test mode (in test mode we dont want logs)
		bool isInTestMode;

		//output path for logfiles
		std::string outputPath;

		// local time for output path
		std::string localTime;

		//Filewrite for logfiles
		LogFileWriter *logwriter;

		//some initialization value to overcome null pointer exceptions that happen at first bbdelay change event
		Bbdelay* currentBbdelay;

		//store all bbdelay characteristics since the last dispatch
		std::vector<Bbdelay*> bbDelayVector;

		//and store the timestamps (in seconds) of the bbdelays
		std::vector<simtime_t> bbTimeVector;

	public:
		CDSQ();
		CDSQ(bool testModeEnabled);
		virtual ~CDSQ();
		virtual void bbDelayChanged(Bbdelay* newBbdelay, simtime_t currentSimtime);
		friend class CDSQ_test; // need for tests
		friend class CRSQ_test; // need for tests

	protected:
		virtual void initialize();
		virtual void writeLogToHarddrive(MyPacket* job, std::string filename, simtime_t currentDelay);

		virtual void handleMessage(cMessage* msg);
		virtual void handleControlMessage(cMessage* msg);

		virtual simtime_t calculateBackboneDelay(simtime_t currentSimtime);
		virtual simtime_t calculateRestBackboneDelay(MyPacket* job, simtime_t currentSimtime);

		virtual void scheduleNewSendHeadAndDeletePacket(simtime_t currentSimtime);
		virtual void scheduleNewSendHeadAndDeletePacket(simtime_t currentSimtime, simtime_t delay);
		virtual void manipulateSelfMessageProcess(simtime_t currentSimtime);

		virtual void addPacketToQueue(MyPacket* job, simtime_t currentSimtime);
		virtual void dispatch(simtime_t currentSimtime);
}
;
}
;

#endif /* CDSQ_H_ */

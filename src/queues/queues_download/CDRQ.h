/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef CDRQ_H_
#define CDRQ_H_

#include "MyPacket_m.h"
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

class CDRQ : public cSimpleModule, public IQueue, public IBbdelayQueue{

	private:
		simsignal_t signalBbdelay;
		cPacketQueue* queue;
		SendHeadAndDeletePacket *currSendHeadCDRQPacket;
		bool isInTestMode;

		//output path for logfiles
		std::string outputPath;
		std::string localTime;

		//Filewrite for logfiles
		LogFileWriter *logwriter;

		//some initialization value to overcome null pointer exceptions that happen at first bbdelay change event
		Bbdelay* currentBbdelay;

		//store all bbdelay characteristics since the last dispatch
		std::vector<Bbdelay*> bbDelayVector;

		//and store the timestamps (in nanoseconds) of the bbdelays
		std::vector<simtime_t> bbTimeVector;

	public:
		CDRQ();
		CDRQ(bool testModeEnabled);
		virtual ~CDRQ();
		virtual void bbDelayChanged(Bbdelay* newBbdelay, simtime_t currentSimtime);
		friend class CDRQ_test; // need for tests
		friend class CRRQ_test; // need for tests

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

#endif /* CDRQ_H_ */

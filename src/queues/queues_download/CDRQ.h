/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */
/**
 * Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#ifndef CDRQ_H_
#define CDRQ_H_

#include "MyPacket_m.h"
#include "SendHeadAndDeletePacket_m.h"
#include "Preamble.h"
#include "Bbdelay.h"
#include "IBbdelayQueue.h"
#include "IQueue.h"

#include <omnetpp.h>
#include <string.h>

namespace projekt {

class CDRQ : public cSimpleModule, public IQueue, public IBbdelayQueue {

	private:
		simsignal_t signalBbdelay;
		cPacketQueue* queue;
		SendHeadAndDeletePacket* currSendHeadCDRQPacket;

		//some initialization value to overcome null pointer exceptions that happen at first bbdelay change event
		Bbdelay* currentBbdelay;

		//store all bbdelay characteristics since the last dispatch
		std::vector<Bbdelay*> bbDelayVector;

		//and store the timestamps (in nanoseconds) of the bbdelays
		std::vector<simtime_t> bbTimeVector;

	public:
		CDRQ();
		virtual ~CDRQ();
		virtual void bbDelayChanged(Bbdelay* newBbdelay);
		friend class CDRQ_test; // need for tests
		friend class CRRQ_test; // need for tests

	protected:
		virtual void initialize();

		virtual void handleMessage(cMessage* msg);
		virtual void handleControlMessage(cMessage* msg);

		virtual simtime_t calculateBackboneDelay();
		virtual simtime_t calculateRestBackboneDelay(MyPacket* job);

		virtual void scheduleNewSendHeadAndDeletePacket();
		virtual void scheduleNewSendHeadAndDeletePacket(simtime_t delay);
		virtual void manipulateSelfMessageProcess();

		virtual void addPacketToQueue(MyPacket* job);
		virtual void dispatch();
}
;
}
;

#endif /* CDRQ_H_ */

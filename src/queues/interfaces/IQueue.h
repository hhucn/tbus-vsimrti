/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 28.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef IQUEUE_H_
#define IQUEUE_H_
namespace projekt {

#include "MyPacket_m.h"

#include <omnetpp.h>
#include <string.h>

class IQueue {

	protected:
		virtual void initialize() = 0;
		virtual void writeLogToHarddrive(MyPacket* job, std::string filename, simtime_t currentDelay) = 0;

		virtual void handleMessage(cMessage* msg) = 0;
		virtual void handleControlMessage(cMessage* msg) = 0;

		virtual void scheduleNewSendHeadAndDeletePacket(simtime_t currentSimtime) = 0;
		virtual void scheduleNewSendHeadAndDeletePacket(simtime_t currentSimtime, simtime_t delay) = 0;
		virtual void manipulateSelfMessageProcess(simtime_t currentSimtime) = 0;

		virtual void addPacketToQueue(MyPacket* job, simtime_t currentSimtime) = 0;
}
;
}
;

#endif /* IQUEUE_H_ */

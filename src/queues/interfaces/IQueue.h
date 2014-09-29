/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 28.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

/**
 * Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
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

		virtual void handleMessage(cMessage* msg) = 0;

//		virtual void scheduleNewSendHeadAndDeletePacket() = 0;
		virtual void scheduleNewSendHeadAndDeletePacket(simtime_t delay) = 0;
		virtual void manipulateSelfMessageProcess() = 0;

		virtual void addPacketToQueue(cMessage* msg) = 0;
};

};

#endif /* IQUEUE_H_ */

/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "Queue.h"

namespace projekt {

Define_Module(Queue)
;

/**
 * default constructor
 */
Queue::Queue() {
	queue = new cPacketQueue();
}

/**
 * default destructor
 */
Queue::~Queue() {
	delete (queue);
}

/**
 * just send all incoming packets without delay to out gate
 * @param msg
 */
void Queue::handleMessage(cMessage *msg) {
	sendDelayed(msg, SIMTIME_ZERO, "out");
}

}
/* namespace projekt */

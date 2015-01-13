//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "TbusChannelControl.h"
#include "ChannelControl.h"
#include "TbusMobilePHY.h"
#include "ModuleAccess.h"
#include "TbusQueueControl.h"

Define_Module(TbusMobilePHY);

/**
 * Constructor.
 * Invalidate host reference.
 */
TbusMobilePHY::TbusMobilePHY() {
	myHostRef = NULL;
}

/**
 * Empty destructor.
 */
TbusMobilePHY::~TbusMobilePHY() {}

/**
 * Simulation initialization.
 * - Stage 0: Gate id assignment, subscribe to position updates, get references
 * - Stage 2: Register IP
 * @param stage Init stage level
 */
void TbusMobilePHY::initialize(int stage) {
	ChannelAccess::initialize(stage);

	if (stage == 0) {
		upperLayerIn 	= findGate("upperLayerIn");
		upperLayerOut 	= findGate("upperLayerOut");
		radioIn 		= findGate("radioIn");

		nb->subscribe(this, NF_HOSTPOSITION_UPDATED);

		tbusCC = check_and_cast<TbusChannelControl*>(ChannelControl::get());
		queueControl = ModuleAccess<TbusQueueControl>("queueControl").get();
	} else if (stage == 2) {
		// Register ip address at channel control
		tbusCC->registerIP(myHostRef);
	}
}

/**
 * Handle message from upper layer.
 * @param msg Message to handle
 */
void TbusMobilePHY::handleUpperMessage(cMessage* msg) {
	sendToChannel(msg);
}

/**
 * Handle message from lower layer.
 * @param msg Message to handle
 */
void TbusMobilePHY::handleLowerMessage(cMessage* msg) {
	send(msg, upperLayerOut);
}

/**
 * Handle incoming message.
 * @param msg Message to handle
 */
void TbusMobilePHY::handleMessage(cMessage* msg) {
	EV << "TbusRadioPHY received message on " << msg->getArrivalGate()->getName() << endl;
	if (msg->isSelfMessage()) {
		// Self message
		delete msg;
	} else if (msg->arrivedOn(upperLayerIn)) {
		// Message from upper layer
		handleUpperMessage(msg);
	} else {
		// Message from lower layer
		handleLowerMessage(msg);
	}
}

/**
 * Send message to channel via channel control.
 * @param msg Message to send
 */
void TbusMobilePHY::sendToChannel(cMessage* msg) {
	tbusCC->sendToChannel(msg, myHostRef);
}

/**
 * Receives host position changes and informs queue control to update network characteristics.
 * @param category ChangeNotification category
 * @param details ChangeNotification details
 * @deprecated VSimRTI's extended mobility information is used instead!
 */
void TbusMobilePHY::receiveChangeNotification(int category, const cObject *details) {
	// We use VSimRTI's extended mobility information
//	if (category == NF_HOSTPOSITION_UPDATED) {
//		if (myHostRef) {
//			queueControl->updateQueues(myHostRef->pos);
//		}
//	}
}

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
#include "TbusQueueControlInfo.h"

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
 * - Stage 3: Register IP
 * @param stage Init stage level
 */
void TbusMobilePHY::initialize(int stage) {
	ChannelAccess::initialize(stage);

	if (stage == 0) {
		upperLayerIn 	= findGate("upperLayerIn");
		upperLayerOut 	= findGate("upperLayerOut");
		radioIn 		= findGate("radioIn");

		tbusCC = check_and_cast<TbusChannelControl*>(ChannelControl::get());
	} else if (stage == 3) {
		// Register ip address at channel control
		tbusCC->registerIP(myHostRef);
	}
}

/**
 * Handle message from upper layer.
 * Removes control info and forwards message.
 * @param msg Message to handle
 */
void TbusMobilePHY::handleUpperMessage(cMessage* msg) {
	// Remove control info
	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(msg->removeControlInfo());
	delete controlInfo;

	sendToChannel(msg);
}

/**
 * Handle message from lower layer.
 * Adds control info and forwards message.
 * Only works when queue control is online.
 * @param msg Message to handle
 */
void TbusMobilePHY::handleLowerMessage(cMessage* msg) {
	msg->setControlInfo(new TbusQueueControlInfo());
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

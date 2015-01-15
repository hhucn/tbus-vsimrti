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

#include "TbusInetPHY.h"
#include "ChannelControl.h"

Define_Module(TbusInetPHY)

/**
 * Empty constructor.
 */
TbusInetPHY::TbusInetPHY() {}

/**
 * Empty destructor.
 */
TbusInetPHY::~TbusInetPHY() {}

/**
 * Simulation initialization.
 * - Stage 0: Gate id assignment, get references
 * - Stage 2: Register IP
 * @param stage Init stage level
 */
void TbusInetPHY::initialize(int stage) {
	ChannelAccess::initialize(stage);

	if (stage == 0) {
		upperLayerIn 	= findGate("upperLayerIn");
		upperLayerOut 	= findGate("upperLayerOut");
		radioIn 		= findGate("radioIn");

		tbusCC = check_and_cast<TbusChannelControl*>(ChannelControl::get());
	} else if (stage == 2) {
		// Register ip address at channel control
		tbusCC->registerIP(myHostRef);
	}
}

/**
 * Handle message from upper layer.
 * @param msg Message to handle
 */
void TbusInetPHY::handleUpperMessage(cMessage* msg) {
	sendToChannel(msg);
}

/**
 * Handle message from lower layer.
 * @param msg Message to handle
 */
void TbusInetPHY::handleLowerMessage(cMessage* msg) {
	send(msg, upperLayerOut);
}

/**
 * Handle incoming message.
 * @param msg Message to handle
 */
void TbusInetPHY::handleMessage(cMessage* msg) {
	EV << "TbusInetPHY received message on " << msg->getArrivalGate()->getName() << endl;
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
void TbusInetPHY::sendToChannel(cMessage* msg) {
	tbusCC->sendToChannel(msg, myHostRef);
}

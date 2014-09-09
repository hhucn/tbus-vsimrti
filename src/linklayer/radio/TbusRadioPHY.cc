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
#include "TbusRadioPHY.h"

Define_Module(TbusRadioPHY);

TbusRadioPHY::TbusRadioPHY() {
    // TODO Auto-generated constructor stub

}

TbusRadioPHY::~TbusRadioPHY() {
    // TODO Auto-generated destructor stub
}

/**
 * Initializes the module
 * @param stage init stage level
 */
void TbusRadioPHY::initialize(int stage) {
	ChannelAccess::initialize(stage);

	if (stage == 0) {
		uppergateIn 	= findGate("upperInGate");
		uppergateOut 	= findGate("upperOutGate");
		radioIn 		= findGate("radioIn");

		nb->subscribe(this, NF_HOSTPOSITION_UPDATED);

		tbusCC = check_and_cast<TbusChannelControl*>(ChannelControl::get());
	}
}

void TbusRadioPHY::finish() {

}

///**
// * Get the hosts coordinates as assigned by the mobility module
// * @return The hosts coordinates
// */
//const Coord TbusRadioPHY::getHostCoord() {
//	return myHostRef->pos;
//}

void TbusRadioPHY::handleSelfMessage(cMessage* msg) {
	// TODO Do a barrel roll
}

void TbusRadioPHY::handleUpperMessage(cMessage* msg) {
	Enter_Method("handleUpperMessage()");

	std::cout << "TbusRadio received message: " << msg << endl;
//
//	// TODO Calculate delay with position here
//
	sendToChannel(msg);
}

void TbusRadioPHY::handleLowerMessage(cMessage* msg) {
	Enter_Method("handleLowerMessage()");

//
//	// TODO Calculate Delay with position here
//
	send(msg, uppergateOut);
}

void TbusRadioPHY::handleMessage(cMessage* msg) {
	std::cout << "call1" << endl;
	if (msg->isSelfMessage()) {
		// Self message
		handleSelfMessage(msg);
	} else if (msg->arrivedOn("upperInGate")) {
		// Message from upper layer
		handleUpperMessage(msg);
	} else {
		// Message from lower layer
		handleLowerMessage(msg);
	}
}

void TbusRadioPHY::sendToChannel(cMessage* msg) {
	tbusCC->sendToChannel(msg);
}

void TbusRadioPHY::receiveChangeNotification(int category, const cObject *details) {
	if (category == NF_HOSTPOSITION_UPDATED) {
		// TODO: Update queues for host position
		std::cout << "TbusRadio receive change notification!" << std::endl;
	}
}

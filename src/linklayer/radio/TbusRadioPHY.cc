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
#include "ModuleAccess.h"
#include "TbusQueueControl.h"

Define_Module(TbusRadioPHY);

TbusRadioPHY::TbusRadioPHY() {
	myHostRef = NULL;
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

void TbusRadioPHY::finish() {
}

void TbusRadioPHY::handleSelfMessage(cMessage* msg) {
	// TODO Do a barrel roll
}

void TbusRadioPHY::handleUpperMessage(cMessage* msg) {
	Enter_Method("handleUpperMessage()");
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
	send(msg, upperLayerOut);
}

void TbusRadioPHY::handleMessage(cMessage* msg) {
	EV << "TbusRadioPHY received message on " << msg->getArrivalGate()->getName() << endl;
	if (msg->isSelfMessage()) {
		// Self message
		handleSelfMessage(msg);
	} else if (msg->arrivedOn(upperLayerIn)) {
		// Message from upper layer
		handleUpperMessage(msg);
	} else {
		// Message from lower layer
		handleLowerMessage(msg);
	}
}

void TbusRadioPHY::sendToChannel(cMessage* msg) {
	tbusCC->sendToChannel(msg, myHostRef);
}

void TbusRadioPHY::receiveChangeNotification(int category, const cObject *details) {
	if (category == NF_HOSTPOSITION_UPDATED) {
		if (myHostRef) {
			queueControl->updateQueues(myHostRef->pos);
		}
		EV << "TbusRadio receive change notification!" << std::endl;
	}
}

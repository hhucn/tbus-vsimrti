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

#include <cobjectfactory.h>
#include <cregistrationlist.h>
#include "NotifierConsts.h"
#include <onstartup.h>
#include <regmacros.h>
#include <simutil.h>
#include <TbusChannelControl.h>
#include <TbusRadio.h>

Define_Module(TbusRadio);

TbusRadio::TbusRadio() {
    // TODO Auto-generated constructor stub

}

TbusRadio::~TbusRadio() {
    // TODO Auto-generated destructor stub
}

/**
 * Initializes the module
 * @param stage init stage level
 */
void TbusRadio::initialize(int stage) {
	ChannelAccess::initialize(stage);

	if (stage == 0) {
		uppergateIn 	= findGate("upperInGate");
		uppergateOut 	= findGate("upperOutGate");
		radioIn 		= findGate("radioIn");

		nb->subscribe(this, NF_HOSTPOSITION_UPDATED);
	}
}

void TbusRadio::finish() {

}

/**
 * Get the hosts coordinates as assigned by the mobility module
 * @return The hosts coordinates
 */
const Coord TbusRadio::getHostCoord() {
	return myHostRef->pos;
}

void TbusRadio::handleSelfMessage(cMessage* msg) {
	// TODO Do a barrel roll
}

void TbusRadio::handleUpperMessage(cMessage* msg) {
	Enter_Method("handleUpperMessage()");

	TbusAirFrame* airFrame = new TbusAirFrame();
	drop(msg);
	airFrame->encapsulate((cPacket*) msg);

	airFrame->setClientMessage(true);
	take(airFrame);

	// TODO Calculate delay with position here

	drop(airFrame);
	sendToChannel(airFrame);
}

void TbusRadio::handleLowerMessage(cMessage* msg) {
	Enter_Method("handleLowerMessage()");

	cMessage* innerMsg = (cMessage*) ((cPacket*) msg)->decapsulate();
	dropAndDelete(msg);

	// TODO Calculate Delay with position here

	// Send to the upper layer
	send(innerMsg, uppergateOut);
}

void TbusRadio::handleMessage(cMessage* msg) {
	take (msg);
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

void TbusRadio::sendToChannel(TbusAirFrame* msg) {
	TbusChannelControl* tbusCc = static_cast<TbusChannelControl*>(cc);

	tbusCc->sendToChannel(msg);
}

void TbusRadio::receiveChangeNotification(int category, const cPolymorphic *details) {
	if (category == NF_HOSTPOSITION_UPDATED) {
		// TODO: Update queues for host position
		std::cout << "TbusRadio receive change notification!" << std::endl;
	}
}

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
#include "omnetpp.h"

Define_Module(TbusChannelControl);

TbusChannelControl::TbusChannelControl() {
}

TbusChannelControl::~TbusChannelControl() {
}

void TbusChannelControl::initialize(int stage) {
	ChannelControl::initialize();

	if (stage == 0) {
		routerInGate  = findGate("routerInGate");
		routerOutGate = findGate("routerOutGate");
	}
}

ChannelControl::HostRef TbusChannelControl::registerHost(cModule *host, const Coord& initialPos, cGate *radioInGate) {
	Enter_Method_Silent();
	ChannelControl::HostRef hostRef = ChannelControl::registerHost(host, initialPos);

	EV << "Registered Host: " << host << std::endl;

	return hostRef;
}

void TbusChannelControl::handleMessage(cMessage* msg) {
	// Only handle router messages
	if (msg->arrivedOn(routerInGate)) {
//		if (TbusMessage* message = dynamic_cast<TbusMessage*>(msg)) {
//			const char* hostName = message->getReceiverHostName();
//
//			for (HostList::iterator it = hosts.begin(); it != hosts.end(); ++it) {
//
//				//TODO: Cast host as MobileNode, access ipAddress and compare
//
//				HostEntry h = *it;
//
//				if (strcmp(hostName, h.host->getFullName()) == 0) {
//					sendToClient(message, &h);
//					return;
//				}
//			}
//		}
	}
}

/**
 * Sends the AirFrame to the routers input gate
 * @param airFrame The AirFrame to send
 */
//void TbusChannelControl::sendToRouter(TbusAirFrame* airFrame) {
//	Enter_Method("sendToRouter()");
////	take(airFrame);
////
////	TbusMessage* message = dynamic_cast<TbusMessage*>(airFrame->decapsulate());
////	delete airFrame;
////
////	ASSERT(message != NULL);
////
////	send(message, routerOutGate);
//}

/**
 * Sends the AirFrame to the given clients radio input gate
 * @param airFrame The AirFrame to send
 * @param client The client to send to
 */
//void TbusChannelControl::sendToClient(TbusMessage* message, HostRef client) {
////	TbusAirFrame* airFrame = new TbusAirFrame();
////	airFrame->encapsulate(message);
////
////	sendDirect(airFrame, client->radioInGate);
//}

/**
 * Sends the message to the corresponding receiver
 * @param msg The message to send
 */
void TbusChannelControl::sendToChannel(cMessage* msg) {
	delete msg;
//	if (TbusAirFrame* airFrame = dynamic_cast<TbusAirFrame*>(msg)) {
//		sendToRouter(airFrame);
//	} else {
//		std::cout << "ChannelControl received invalid message!" << std::endl;
//	}
}

/**
 * Hook for updating host positions
 * @param h The host to update
 * @param pos New position
 */
void TbusChannelControl::updateHostPosition(HostRef h, const Coord& pos) {
    ChannelControl::updateHostPosition(h, pos);

    EV << "New Host Position (" << pos.x << "," << pos.y << ") for host " << h->host->getFullName() << std::endl;
}

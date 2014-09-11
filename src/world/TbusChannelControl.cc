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
	// TODO: Handle router messages
}

/**
 * Sends the message to the corresponding receiver
 * @param msg The message to send
 */
void TbusChannelControl::sendToChannel(cMessage* msg, HostRef h) {
	Enter_Method_Silent();
	take(msg);

		EV << "Sending message from " << msg->getSenderModule() << std::endl;

	for (HostList::iterator it = hosts.begin(); it != hosts.end(); ++it) {
		if (strcmp(h->host->getFullName(), it->host->getFullName()) != 0) {
			sendDirect(msg->dup(), it->radioInGate);
		}
	}

	//TODO/DISCUSSION: Send the message to router?

	drop(msg);
	delete msg;
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

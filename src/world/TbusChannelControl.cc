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
#include "IPAddressResolver.h"
#include "IInterfaceTable.h"
#include "InterfaceEntry.h"
#include "IPv4InterfaceData.h"
#include "IPDatagram_m.h"
#include "TbusRadioPHY.h"
#include <map>

Define_Module(TbusChannelControl);

/**
 * Empty constructor
 */
TbusChannelControl::TbusChannelControl() {
}

/**
 * Empty destructor
 */
TbusChannelControl::~TbusChannelControl() {}

/**
 * Simulation initialization.
 * Stores gateIds for in and output
 */
void TbusChannelControl::initialize() {
	ChannelControl::initialize();

	routerInGate  = findGate("routerInGate");
	routerOutGate = findGate("routerOutGate");
}

/**
 * Register hostRef's first NIC IP from a NIC that is no loopback.
 * This is done for an IP to Host mapping in O(1).
 * @param hostRef Host to register
 */
void TbusChannelControl::registerIP(ChannelControl::HostRef hostRef) {
	IInterfaceTable* interfaceTable = IPAddressResolver().interfaceTableOf(hostRef->host);
	InterfaceEntry* ie = NULL;

	for (int i = 0; i < interfaceTable->getNumInterfaces(); i++) {
		ie = interfaceTable->getInterface(i);

		if (!ie->isLoopback()) {
			break;
		}
	}

	ASSERT2(ie != NULL, "No non-loopback interface found!");

	hostMap.insert(ip2host(ie->ipv4Data()->getIPAddress(), hostRef));

	EV << "Registered " << hostRef->host->getFullName() << " with address " << ie->ipv4Data()->getIPAddress() << endl;
}

/**
 * Handles messages send to TbusChannelControl via ordinary connections.
 * @param msg Message to send
 */
void TbusChannelControl::handleMessage(cMessage* msg) {
	// TODO: Handle router messages
}

/**
 * Sends duplicates of the message to the corresponding receiver(s).
 * Only supports Broadcast and Unicast right now.
 * @param msg Message to send
 * @param h Host reference from sender
 */
void TbusChannelControl::sendToChannel(cMessage* msg, HostRef h) {
	Enter_Method_Silent();
	take(msg);

	IPDatagram* packet = check_and_cast<IPDatagram*>(msg);

	if (packet) {
		IPAddress ip = packet->getDestAddress();

		if (ip == IPAddress::ALLONES_ADDRESS) {
			// Broadcast
			EV << "Broadcasting message " << msg->getFullName() << endl;

			for (HostList::iterator it = hosts.begin(); it != hosts.end(); ++it) {
				if (strcmp(h->host->getFullName(), it->host->getFullName()) != 0) {
					sendDirect(msg->dup(), it->radioInGate);
				}
			}
		} else {
			// We suppose unicast here
			EV << "Unicasting message " << msg->getFullName() << endl;

			ChannelControl::HostRef destinationHost = hostMap.find(ip)->second;

			sendDirect(msg->dup(), destinationHost->radioInGate);
		}
	} else {
		opp_error("Tbus Channel Control received non-IPDatagram packet!");
	}

	drop(msg);
	delete msg;
}

/**
 * Hook for updating host positions.
 * @param h Host to update
 * @param pos New position
 */
void TbusChannelControl::updateHostPosition(HostRef h, const Coord& pos) {
    ChannelControl::updateHostPosition(h, pos);
    EV << "New Host Position (" << pos.x << "," << pos.y << ") for host " << h->host->getFullName() << std::endl;
}

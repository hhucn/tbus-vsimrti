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
#include "TbusMobilePHY.h"
#include "VSimRTIAppPacket_m.h"
#include "TbusWorldView.h"
#include <map>

#ifdef TBUS_DEBUG
Register_GlobalConfigOption(CFGID_TBUS_DATABASE_FILE_DEBUG, "tbus-database", CFG_FILENAME, "", "Filename of TBUS database")
#endif /* TBUS_DEBUG */

Define_Module(TbusChannelControl);

/**
 * Empty constructor
 */
TbusChannelControl::TbusChannelControl() {}

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

#ifdef TBUS_DEBUG
	sqlite3* database;
	sqlite3_open_v2(ev.getConfig()->getAsFilename(CFGID_TBUS_DATABASE_FILE_DEBUG).c_str(), &database, SQLITE_OPEN_READONLY, NULL);

	sqlite3_stmt* statement;
	sqlite3_prepare_v2(database, "select timestamp from download order by timestamp asc;", -1, &statement, NULL);

	uint64_t time;
	uint64_t now = simTime().inUnit(SIMTIME_NS);

	while (sqlite3_step(statement) == SQLITE_ROW) {
		time = sqlite3_column_int64(statement, 0);

		if (time >= now) {
			scheduleAt(SimTime(time, SIMTIME_NS), new cMessage(NULL, 0));
		}
	}

	sqlite3_finalize(statement);

	sqlite3_close(database);
#endif /* TBUS_DEBUG */
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

	bool result = hostMap.insert(ip2host(ie->ipv4Data()->getIPAddress(), hostRef)).second;
	if (!result) {
		EV << "ERROR: Could not register host " << hostRef->host->getFullName() << " to TbusChannelControl, this host will not receive any data" << endl;
	} else {
		EV << "Registered " << hostRef->host->getFullName() << " with address " << ie->ipv4Data()->getIPAddress() << endl;
	}
}

/**
 * Handles messages send to TbusChannelControl via ordinary connections.
 * @param msg Message to send
 */
void TbusChannelControl::handleMessage(cMessage* msg) {
#ifdef TBUS_DEBUG
	if (msg->isSelfMessage()) {
		TbusWorldView::getInstance()->performUpdateRound();
	}
	delete msg;
#endif /* TBUS_DEBUG */
}

/**
 * Sends duplicates of the message to the corresponding receiver(s).
 * Only supports Broadcast and Unicast right now.
 * @param msg Message to send
 * @param h Host reference from sender
 */
void TbusChannelControl::sendToChannel(cMessage* msg, ChannelControl::HostRef h) {
	Enter_Method_Silent();
	take(msg);

	// We only support VSimRTIAppPacket from now on
	IPDatagram* packet = check_and_cast<IPDatagram*>(msg);
//	VSimRTIAppPacket* packet = check_and_cast<VSimRTIAppPacket*>(msg);

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
			ip2hostMap::iterator dest = hostMap.find(ip);
			if (dest == hostMap.end()) {
				EV << "ERROR: Cannot find host with IP address " << ip << ", discarding packet" << endl;
			} else {
				EV << "Unicasting message " << msg->getFullName() << " to " << ip << endl;
				sendDirect(msg->dup(), dest->second->radioInGate);
			}
		}
	} else {
		throw cRuntimeError("Tbus Channel Control received non-IPDatagram packet!");
	}

	drop(msg);
	delete msg;
}

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

#include "TbusInetMAC.h"
#include "IpAddressHelper.h"
#include "IInterfaceTable.h"
#include "InterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "RoutingTableAccess.h"
#include "IPv4InterfaceData.h"

Define_Module(TbusInetMAC)

/**
 * Empty constructor
 */
TbusInetMAC::TbusInetMAC() {}

/**
 * Empty destructor
 */
TbusInetMAC::~TbusInetMAC() {}

/**
* Add TBus NIC to interface table.
 * - Stage 0: MAC address is generated, assigned and added to interface table.
 * - Stage 1: IP address is generated, assigned and added to routing table.
 * @param stage Initialization stage
 */
void TbusInetMAC::initialize(int stage) {
	if (stage == 0) {
		interfaceEntry = new InterfaceEntry();
		macAddress = MACAddress::generateAutoAddress();

		interfaceEntry->setName("tbus0");
		interfaceEntry->setMACAddress(macAddress);
		interfaceEntry->setInterfaceToken(macAddress.formInterfaceIdentifier());

		interfaceEntry->setMulticast(true);
		interfaceEntry->setBroadcast(true);

		interfaceEntry->setDown(false);

		interfaceEntry->setMtu(1500);
		interfaceEntry->setDatarate(100.0);

		IInterfaceTable* interfaceTable = InterfaceTableAccess().getIfExists();
		if (interfaceTable) {
			interfaceTable->addInterface(interfaceEntry, this);
		}

		upperLayerIn = findGate("upperLayerIn");
		upperLayerOut = findGate("upperLayerOut");
		lowerLayerIn = findGate("lowerLayerIn");
		lowerLayerOut = findGate("lowerLayerOut");
	} else if (stage == 2) {
		// Set IP Address
		interfaceEntry->ipv4Data()->setIPAddress(IpAddressHelper::getNextRsuIpAddress());
		interfaceEntry->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS);

		// Add default route
		IRoutingTable* routingTable = RoutingTableAccess().getIfExists();
		if (routingTable) {
			routingTable->addRoute(IpAddressHelper::getDefaultRoute(interfaceEntry));
		}

		EV << "Interface "<< this->getParentModule()->getFullName() << " now has IP address " << interfaceEntry->ipv4Data()->getIPAddress() << endl;
	}
}

/**
 * Forwards message msg to the next layer
 * @param msg Message to forward
 */
void TbusInetMAC::handleMessage(cMessage* msg) {
	if (msg->arrivedOn(upperLayerIn)) {
		send(msg, lowerLayerOut);
	} else if (msg->arrivedOn(lowerLayerIn)) {
		send(msg, upperLayerOut);
	}
}

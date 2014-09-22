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

#include <TbusRadioMAC.h>
#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "INETDefs.h"
#include "opp_utils.h"
#include "IPAddress.h"
#include "IPv4InterfaceData.h"
#include "RoutingTableAccess.h"
#include "IRoutingTable.h"
#include "IPRoute.h"
#include <string.h>
#include <algorithm>
#include <locale>

Define_Module(TbusRadioMAC);

int TbusRadioMAC::ipByte = 0;

TbusRadioMAC::TbusRadioMAC() {
}

TbusRadioMAC::~TbusRadioMAC() {
}

void TbusRadioMAC::receiveChangeNotification(int category, const cObject* details) {
	EV << "Interface created!" << std::endl;
}

/**
 * Adds our TBus NIC to the interface table
 */
void TbusRadioMAC::initialize(int stage) {
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

		nb = NotificationBoardAccess().getIfExists();
		nb->subscribe(this, NF_INTERFACE_CREATED);
		nb->subscribe(this, NF_SUBSCRIBERLIST_CHANGED);

		upperLayerIn = findGate("upperLayerIn");
		upperLayerOut = findGate("upperLayerOut");
		lowerLayerIn = findGate("lowerLayerIn");
		lowerLayerOut = findGate("lowerLayerOut");
	} else if (stage == 2) {
		// Set IP Address
		interfaceEntry->ipv4Data()->setIPAddress(IPAddress(192, 168, 0, ++TbusRadioMAC::ipByte));
		interfaceEntry->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS);

		// Add default route
		IRoutingTable* routingTable = RoutingTableAccess().getIfExists();
		if (routingTable) {
			IPRoute* route = new IPRoute();
			route->setHost(IPAddress(192, 168, 0, 0));
			route->setNetmask(IPAddress(255, 255, 255, 0));
			route->setInterface(interfaceEntry);
			route->setSource(IPRoute::MANUAL);

			routingTable->addRoute(route);
		}

		EV << "Interface "<< this->getParentModule()->getFullName() << " now has IP address " << interfaceEntry->ipv4Data()->getIPAddress() << endl;
	}
}

void TbusRadioMAC::handleMessage(cMessage* msg) {
	if (msg->arrivedOn(upperLayerIn)) {
		send(msg, lowerLayerOut);
	} else if (msg->arrivedOn(lowerLayerIn)) {
		send(msg, upperLayerOut);
	}
}

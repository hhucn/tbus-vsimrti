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

#include "TbusMobileMAC.h"
#include "TbusQueueControlInfo.h"
#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "INETDefs.h"
#include "opp_utils.h"
#include "IPAddress.h"
#include "IPv4InterfaceData.h"
#include "RoutingTableAccess.h"
#include "IRoutingTable.h"
#include "IPRoute.h"
#include "IpAddressHelper.h"

Define_Module(TbusMobileMAC);

/**
 * Empty constructor.
 */
TbusMobileMAC::TbusMobileMAC() {}

/**
 * Empty destructor.
 */
TbusMobileMAC::~TbusMobileMAC() {}

/**
 * Add TBus NIC to interface table.
 * - Stage 0: MAC address is generated, assigned and added to interface table.
 * - Stage 1: IP address is generated, assigned and added to routing table.
 * @param stage Initialization stage
 */
void TbusMobileMAC::initialize(int stage) {
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
		interfaceEntry->ipv4Data()->setIPAddress(IpAddressHelper::getNextVehicleIpAddress());
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
 * Handle incoming messages.
 * Control info is removed and message is sent to corresponding lower/upper layer.
 * @param msg Message to handle
 */
void TbusMobileMAC::handleMessage(cMessage* msg) {
	cObject* controlInfo = msg->removeControlInfo();

	EV << "removed control info " << controlInfo << std::endl;

	if (msg->arrivedOn(upperLayerIn)) {
		// Add control info
		msg->setControlInfo(new TbusQueueControlInfo());
		send(msg, lowerLayerOut);
	} else if (msg->arrivedOn(lowerLayerIn)) {
		// Remove control info
		TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(msg->removeControlInfo());
		delete controlInfo;

		send(msg, upperLayerOut);
	}
}

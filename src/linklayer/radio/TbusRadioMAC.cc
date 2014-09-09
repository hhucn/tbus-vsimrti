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
#include <string.h>
#include <algorithm>
#include <locale>

Define_Module(TbusRadioMAC);

TbusRadioMAC::TbusRadioMAC() {
}

TbusRadioMAC::~TbusRadioMAC() {
}

void TbusRadioMAC::receiveChangeNotification(int category, const cObject* details) {
	std::cout << "Interface created!" << std::endl;
}

/**
 * Adds our TBus NIC to the interface table
 */
void TbusRadioMAC::initialize(int stage) {
	interfaceEntry = new InterfaceEntry();
	macAddress = MACAddress::generateAutoAddress();

	// Erase all non-alnum chars
//	std::string name = getParentModule()->getFullName();
//	name.erase(std::remove_if(name.begin(), name.end(), (int(*)(int)) std::isalnum), name.end());

	interfaceEntry->setName("tbus0");//name.c_str());
	interfaceEntry->setMACAddress(macAddress);
	interfaceEntry->setInterfaceToken(macAddress.formInterfaceIdentifier());

	interfaceEntry->setMulticast(true);
	interfaceEntry->setBroadcast(true);

	interfaceEntry->setDown(false);
	interfaceEntry->setDatarate(12.0);

	interfaceEntry->setMtu(1500);

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
}

void TbusRadioMAC::handleMessage(cMessage* msg) {
	if (msg->arrivedOn(upperLayerIn)) {
		send(msg, lowerLayerOut);
	} else if (msg->arrivedOn(lowerLayerIn)) {
		send(msg, upperLayerOut);
	}
}

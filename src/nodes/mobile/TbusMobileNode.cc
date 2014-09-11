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

#include <TbusMobileNode.h>
#include "InterfaceTableAccess.h"
#include "IInterfaceTable.h"
#include "InterfaceEntry.h"
#include "IPv4InterfaceData.h"

Define_Module(TbusMobileNode)

TbusMobileNode::TbusMobileNode() {
	// TODO Auto-generated constructor stub

}

TbusMobileNode::~TbusMobileNode()
{
	// TODO Auto-generated destructor stub
}

void TbusMobileNode::initialize(int stage) {
	if (stage == 4) {
		IInterfaceTable *ift = InterfaceTableAccess().get();

		for (int i=0;i<ift->getNumInterfaces();i++) {
			InterfaceEntry * ie = ift->getInterface(i);

			if (ie->isLoopback())
			   continue;
			ipAddress = ie->ipv4Data()->getIPAddress();
			break;
		}

		EV << "Node " << this << " assigned ip address " << ipAddress << endl;
	}
}

int TbusMobileNode::numInitStages() const {
	return 5;
}

const IPvXAddress TbusMobileNode::getIpAddress() const {
	return ipAddress;
}

void TbusMobileNode::handleMessage(cMessage* msg) {

}

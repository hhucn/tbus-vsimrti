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

#include <TbusRoutingApplication.h>
#include "UDPControlInfo_m.h"

Define_Module(TbusRoutingApplication);

TbusRoutingApplication::TbusRoutingApplication()
{
	// TODO Auto-generated constructor stub

}

TbusRoutingApplication::~TbusRoutingApplication()
{
	// TODO Auto-generated destructor stub
}

void TbusRoutingApplication::initialize(int stage) {
	if (stage == 0) {
		lowerInputGate  = findGate("lowerInGate");
		lowerOutputGate = findGate("lowerOutGate");
	}
}

void TbusRoutingApplication::handleMessage(cMessage* msg) {
	// Once a message has been received, send it back to the sender

	if (msg->getKind() == UDP_I_ERROR) {
		delete msg;

		EV << "Router discarded erroneous message." << endl;

		return;
	}

	UDPControlInfo* controlInfo = check_and_cast<UDPControlInfo*>(((cPacket*) msg)->getControlInfo());

	EV << "Routing app received message from " << controlInfo->getSrcAddr() << endl;

	// Send a "pong" message back
	IPvXAddress srcAddress = controlInfo->getSrcAddr();
	int srcPort = controlInfo->getSrcPort();
	controlInfo->setSrcAddr(controlInfo->getDestAddr());
	controlInfo->setSrcPort(controlInfo->getDestPort());
	controlInfo->setDestAddr(srcAddress);
	controlInfo->setDestPort(srcPort);

	send(msg, lowerOutputGate);
}

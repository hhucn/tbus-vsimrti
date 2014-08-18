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
#include "world/TbusChannelControl.h"
#include "TbusAirFrame_m.h"

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
	// Once a message has been received, send one back to the sender
	TbusMessage* message = dynamic_cast<TbusMessage*>(msg);

	ASSERT(message != NULL);

	std::cout << "Routing app received message from " << message->getSenderHostName() << std::endl;

	// Send a "pong" message back
	TbusMessage* sendMessage = new TbusMessage();
	sendMessage->setReceiverHostName(message->getSenderHostName());
	sendMessage->setSenderHostName(this->getFullName());

	// Delete the received message, we won't deliver it any further
	delete msg;

	send(sendMessage, lowerOutputGate);
}

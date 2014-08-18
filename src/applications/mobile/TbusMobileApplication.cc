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

#include <TbusMobileApplication.h>
#include "omnetpp.h"
#include "TbusAirFrame_m.h"

Define_Module(TbusMobileApplication)

TbusMobileApplication::TbusMobileApplication() {
	// TODO Auto-generated constructor stub

}

TbusMobileApplication::~TbusMobileApplication() {
	// TODO Auto-generated destructor stub
}

void TbusMobileApplication::initialize(int stage) {
	if (stage == 0) {
			cc = static_cast<TbusChannelControl*>(TbusChannelControl::get());

		lowerInputGate = findGate("lowerInGate");
		lowerOutputGate = findGate("lowerOutGate");
	} else {
		cMessage* msg = new cMessage();
		scheduleAt(simTime() + 1.0, msg);
	}
}

void TbusMobileApplication::handleSelfMessage(cMessage* msg) {
	TbusMessage* message = new TbusMessage();

	// Set the mobile node name as our hostname
	message->setSenderHostName(this->getParentModule()->getFullName());

	send(message, lowerOutputGate);

	// Schedule new self message
	cMessage* selfMsg = new cMessage();
	scheduleAt(simTime() + 1.0, selfMsg);
}

void TbusMobileApplication::handleMessage(cMessage* msg) {
	if (msg->isSelfMessage()) {
		handleSelfMessage(msg);
	} else {
		std::cout << "Node " << this->info() << " has received packet " << msg << std::endl;
	}

	delete msg;
}

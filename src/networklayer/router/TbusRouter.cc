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

#include <router/TbusRouter.h>
#include <omnetpp.h>

Define_Module(TbusRouter);

TbusRouter::TbusRouter() {
	// TODO Auto-generated constructor stub
}

TbusRouter::~TbusRouter() {
}

void TbusRouter::initialize(int stage) {
	if (stage == 0) {
		channelInGate = findGate("channelInGate");
		channelOutGate = findGate("channelOutGate");
		inetInGate = findGate("inetInGate");
		inetOutGate = findGate("inetOutGate");
		upperInGate = findGate("upperInGate");
		upperOutGate = findGate("upperOutGate");
	}
}

void TbusRouter::handleMessage(cMessage* msg) {
	if (msg->arrivedOn(channelInGate)) {
		std::cout << "TbusRouter received message from channel" << std::endl;
		send(msg, upperOutGate);
	} else if (msg->arrivedOn(upperInGate)) {
		std::cout << "TbusRouter received message from upper layer" << std::endl;
		send(msg, channelOutGate);
	} else if (msg->arrivedOn(inetInGate)) {
		std::cout << "TbusRouter received message from inet" << std::endl;
		send(msg, upperOutGate);
	}
}


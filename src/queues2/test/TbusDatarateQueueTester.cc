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

#include <test/TbusDatarateQueueTester.h>
#include "TbusQueueDatarateValue.h"
#include "TbusDatarateQueue.h"
#include "ModuleAccess.h"

template class TbusTestBase<TbusDatarateQueueTester>;

Define_Module(TbusDatarateQueueTester);

TbusDatarateQueueTester::TbusDatarateQueueTester() : TbusTestBase(this) {
	addTest(&TbusDatarateQueueTester::testNormalDatarate);
}

TbusDatarateQueueTester::~TbusDatarateQueueTester() {
	// TODO Auto-generated destructor stub
}

void TbusDatarateQueueTester::initialize() {
	inGate = findGate("inGate");
	outGate = findGate("outGate");

	datarateQueue = ModuleAccess<TbusDatarateQueue>("testQueue").get();

	runNextTest();
}

void TbusDatarateQueueTester::handleMessage(cMessage* msg) {
	if (!msg->isSelfMessage()) {
		EV << "SimTime: " << simTime() << "Expected time:" << expectedTime << std::endl;
		if (simTime() != expectedTime) {
			opp_error("Test failed!");
			delete msg;
		} else {
			EV << "Test finished... Running next test!" << std::endl;
			delete msg;
			runNextTest();
		}
	}
}

void TbusDatarateQueueTester::testNormalDatarate() {
	TbusQueueDatarateValue* datarate = new TbusQueueDatarateValue();
	datarate->datarate = 10.0;
	datarate->droprate = 0.0;
	expectedTime = simTime() + 1;

	datarateQueue->updateValue(datarate);

	cPacket* packet = new cPacket("Testpacket", 0, 1024);
	EV << "Send testpacket with datarate 10.0 at " << simTime() << std::endl;
	send(packet, outGate);
}

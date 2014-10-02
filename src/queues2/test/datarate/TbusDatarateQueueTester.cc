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

#include "TbusDatarateQueueTester.h"
#include "TbusQueueDatarateValue.h"
#include "TbusDatarateQueue.h"
#include "ModuleAccess.h"
#include "testPacket_m.h"

#include <sstream>

template class TbusTestBase<TbusDatarateQueueTester>;

Define_Module(TbusDatarateQueueTester);

TbusDatarateQueueTester::TbusDatarateQueueTester() : TbusTestBase(this) {
	addTest(&TbusDatarateQueueTester::testNormalDatarate, &TbusDatarateQueueTester::handleNormalDatarate, "Normal Datarate Test");
	addTest(&TbusDatarateQueueTester::testFullDrop, &TbusDatarateQueueTester::handleFullDrop, "Full Drop Test");
	addTest(&TbusDatarateQueueTester::testHalfDrop, &TbusDatarateQueueTester::handleHalfDrop, "Half Drop Test");
	addTest(&TbusDatarateQueueTester::testChangingDatarate, &TbusDatarateQueueTester::handleChangingDatarate, "Changing Datarate Test");
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

void TbusDatarateQueueTester::testChangingDatarate() {
	TbusQueueDatarateValue* datarate1 = new TbusQueueDatarateValue();
	datarate1->datarate = 16.0; //16Bitps
	datarate1->droprate = 0.0;

	// After 1 sec, datarate drops to 8Bitps

	TestPacket* packet1 = new TestPacket("ChangingDatarateTestPacket1", 0);
	packet1->setBitLength(32); //Transmission should take 3sec
	packet1->setExpectedArrival(simTime() + 3.0);
	packet1->setDispatchTime(simTime());

	TestPacket* packet2 = new TestPacket("ChangingDatarateTestPacket2", 0);
	packet2->setBitLength(32); //Transmission should take 4sec plus 3sec from the first packet
	packet2->setExpectedArrival(simTime() + 4.0 + 3.0);
	packet2->setDispatchTime(simTime());

	datarateQueue->updateValue(datarate1);
	scheduleAt(simTime() + 1, new cMessage("change.datarate"));

	send(packet1, outGate);
	send(packet2, outGate);
}

void TbusDatarateQueueTester::handleChangingDatarate(cMessage* msg) {
	if (msg->isSelfMessage() && (strcmp(msg->getName(), "change.datarate") == 0)) {
		TbusQueueDatarateValue* datarate2 = new TbusQueueDatarateValue();
		datarate2->datarate = 8.0; //5Bps
		datarate2->droprate = 0.0;

		datarateQueue->updateValue(datarate2);
	} else {
		success = success && (abs(simTime().dbl() - ((TestPacket*) msg)->getExpectedArrival().dbl()) < 0.000001);
		EV << "Changed datarate packet " << msg << " arrived with difference " << abs(simTime().dbl()  - ((TestPacket*) msg)->getExpectedArrival().dbl()) << std::endl;

		if (strcmp(msg->getName(), "ChangingDatarateTestPacket2") == 0) {
			scheduleAt(simTime(), &nextTestMessage);
		}
	}

	delete msg;
}

void TbusDatarateQueueTester::testHalfDrop() {
	TbusQueueDatarateValue* datarate = new TbusQueueDatarateValue();
	datarate->datarate = 10000000.0/8.0; //10Mbit
	datarate->droprate = .5;

	datarateQueue->updateValue(datarate);

	for (uint32_t offset = 0; offset < 100; ++offset) {
		std::ostringstream outStream;
		outStream << "HalfDropTestPacket" << offset;
		TestPacket* packet = new TestPacket(outStream.str().c_str(), 0);

		packet->setByteLength(1024);
		packet->setDispatchTime(simTime() + offset);
		packet->setExpectedArrival(simTime().dbl() + (double) offset + ((double) packet->getBitLength() / datarate->datarate));

		sendDelayed(packet, offset, outGate);
	}

	scheduleAt(simTime() + 100, new cMessage("HalfDropEnd", 0));
}

void TbusDatarateQueueTester::handleHalfDrop(cMessage* msg) {
	static uint32_t arrivals = 0;

	if (!msg->isSelfMessage()) {
		arrivals++;
		EV << "Message " << msg->getName() << " survived!" << std::endl;
		delete msg;
	} else if (msg->isSelfMessage() && (strcmp(msg->getName(), "HalfDropEnd") == 0)){
		delete msg;

		EV << "Test Half Drop: " << arrivals << " out of 100 packets arrived!" << std::endl;
		success = (abs(50 - arrivals) < 7);

		scheduleAt(simTime(), &nextTestMessage);
	}
}

void TbusDatarateQueueTester::testFullDrop() {
	TbusQueueDatarateValue* datarate = new TbusQueueDatarateValue();
	datarate->datarate = 10000000.0/8.0; //10Mbit
	datarate->droprate = 1.0;

	datarateQueue->updateValue(datarate);

	for (uint32_t offset = 0; offset < 100; ++offset) {
		std::ostringstream outStream;
		outStream << "FullDropTestPacket" << offset;
		TestPacket* packet = new TestPacket(outStream.str().c_str(), 0);

		packet->setByteLength(1024);
		packet->setDispatchTime(simTime() + offset);
		packet->setExpectedArrival(simTime().dbl() + (double) offset + ((double) packet->getBitLength() / datarate->datarate));

		sendDelayed(packet, offset, outGate);
	}

	scheduleAt(simTime() + 100, &nextTestMessage);
}

void TbusDatarateQueueTester::handleFullDrop(cMessage* msg) {
	// We don't expect any packets to come through
	success = false;
}

void TbusDatarateQueueTester::testNormalDatarate() {
	TbusQueueDatarateValue* datarate = new TbusQueueDatarateValue();
	datarate->datarate = 10000000.0/8.0; //10Mbit
	datarate->droprate = 0.0;

	datarateQueue->updateValue(datarate);

	for (uint32_t offset = 0; offset < 100; ++offset) {
		std::ostringstream outStream;
		outStream << "TestPacket" << offset;
		TestPacket* packet = new TestPacket(outStream.str().c_str(), 0);

		packet->setByteLength(1024);
		packet->setDispatchTime(simTime() + offset);
		packet->setExpectedArrival(simTime().dbl() + (double) offset + ((double) packet->getBitLength() / datarate->datarate));

		sendDelayed(packet, simTime() + offset, outGate);
	}
}

void TbusDatarateQueueTester::handleNormalDatarate(cMessage* msg) {
	if (!msg->isSelfMessage()) {
		TestPacket* packet = check_and_cast<TestPacket*>(msg);

		if (packet) {
			EV << "Packet " << packet->getName() ;
			if (abs(simTime().dbl() - packet->getExpectedArrival().dbl()) <= 0.000000001) {
				EV << " arrived on time" << std::endl;
				success = success && true;
			} else {
				EV << " arrived " << (simTime() - packet->getExpectedArrival()) << " seconds too late!" << std::endl;
				success = success && false;
			}
		}

		if (strcmp(packet->getName(), "TestPacket99") == 0) {
			scheduleAt(simTime() + 1, &nextTestMessage);
		}
		delete packet;
	}
}

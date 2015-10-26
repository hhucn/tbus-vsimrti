//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusDatarateQueueTester.h"
#include "TbusQueueDatarateValue.h"
#include "TbusDatarateQueue.h"
#include "ModuleAccess.h"
#include "testPacket_m.h"

#include <sstream>

Define_Module(TbusDatarateQueueTester);

/**
 * Initialize test base with current instance and add tests.
 */
TbusDatarateQueueTester::TbusDatarateQueueTester() : TbusTestBase(this) {
	addTest(&TbusDatarateQueueTester::testNormalDatarate, &TbusDatarateQueueTester::handleNormalDatarate, "Normal Datarate Test");
	addTest(&TbusDatarateQueueTester::testFullDrop, &TbusDatarateQueueTester::handleFullDrop, "Full Drop Test");
	addTest(&TbusDatarateQueueTester::testHalfDrop, &TbusDatarateQueueTester::handleHalfDrop, "Half Drop Test");
	addTest(&TbusDatarateQueueTester::testChangingDatarate, &TbusDatarateQueueTester::handleChangingDatarate, "Changing Datarate Test");
}

/**
 * Empty destructor.
 */
TbusDatarateQueueTester::~TbusDatarateQueueTester() {}

/**
 * Simulation initialization
 */
void TbusDatarateQueueTester::initialize() {
	inGate = findGate("inGate");
	outGate = findGate("outGate");

	queue = ModuleAccess<TbusDatarateQueue>("datarateQueue").get();

	runNextTest();
}

/**
 * Test changing datarate.
 */
void TbusDatarateQueueTester::testChangingDatarate() {
	// Reset values
	queue->updateValue(NULL);
	queue->updateValue(NULL);

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

//	datarateQueue->updateValue(datarate1);
	queue->updateValue(datarate1);

	#define CHANGING_DATARATE_END "change.datarate"
	scheduleAt(simTime() + 1, new cMessage(CHANGING_DATARATE_END));

	send(packet1, outGate);
	send(packet2, outGate);
}

/**
 * Message handler for test changing datarate.
 * @param msg Message to handle
 */
void TbusDatarateQueueTester::handleChangingDatarate(cMessage* msg) {
	if (msg->isSelfMessage() && (strcmp(msg->getName(), CHANGING_DATARATE_END) == 0)) {
		TbusQueueDatarateValue* datarate2 = new TbusQueueDatarateValue();
		datarate2->datarate = 8.0; //8Bps
		datarate2->droprate = 0.0;

//		datarateQueue->updateValue(datarate2);
		queue->updateValue(datarate2);
	} else {
		success = success && (abs(simTime().dbl() - ((TestPacket*) msg)->getExpectedArrival().dbl()) < 0.000001);
		EV << "Changed datarate packet " << msg << " arrived with difference " << abs(simTime().dbl()  - ((TestPacket*) msg)->getExpectedArrival().dbl()) << std::endl;

		if (strcmp(msg->getName(), "ChangingDatarateTestPacket2") == 0) {
			scheduleAt(simTime(), &nextTestMessage);
		}
	}

	delete msg;
}

/**
 * Test Queue with 10 MBit/s datarate and 50% droprate.
 */
void TbusDatarateQueueTester::testHalfDrop() {
	// Reset values
	queue->updateValue(NULL);
	queue->updateValue(NULL);

	TbusQueueDatarateValue* datarate = new TbusQueueDatarateValue();
	datarate->datarate = 10000000.0/8.0; //10Mbit
	datarate->droprate = .5;

//	datarateQueue->updateValue(datarate);
	queue->updateValue(datarate);

	for (uint32_t offset = 0; offset < 100; ++offset) {
		std::ostringstream outStream;
		outStream << "HalfDropTestPacket" << offset;
		TestPacket* packet = new TestPacket(outStream.str().c_str(), 0);

		packet->setByteLength(1024);
		packet->setDispatchTime(simTime() + offset);
		packet->setExpectedArrival(simTime().dbl() + (double) offset + ((double) packet->getBitLength() / datarate->datarate));

		sendDelayed(packet, offset, outGate);
	}

	#define HALF_DROP_END "HalfDropEnd"
	scheduleAt(simTime() + 100, new cMessage(HALF_DROP_END, 0));
}

/**
 * Message handler for test with 10MBit/s datarate and 50% droprate.
 * @param msg Message to handle
 */
void TbusDatarateQueueTester::handleHalfDrop(cMessage* msg) {
	static uint32_t arrivals = 0;

	if (!msg->isSelfMessage()) {
		arrivals++;
		EV << "Message " << msg->getName() << " survived!" << std::endl;
		delete msg;
	} else if (msg->isSelfMessage() && (strcmp(msg->getName(), HALF_DROP_END) == 0)){
		delete msg;

		EV << "Test Half Drop: " << arrivals << " out of 100 packets arrived!" << std::endl;
		success = (abs(50 - arrivals) < 7);

		scheduleAt(simTime(), &nextTestMessage);
	}
}

/**
 * Test with 10MBit/s datarate and 100% droprate.
 */
void TbusDatarateQueueTester::testFullDrop() {
	// Reset values
	queue->updateValue(NULL);
	queue->updateValue(NULL);

	TbusQueueDatarateValue* datarate = new TbusQueueDatarateValue();
	datarate->datarate = 10000000.0/8.0; //10Mbit
	datarate->droprate = 1.0;

//	datarateQueue->updateValue(datarate);
	queue->updateValue(datarate);

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

/**
 * Message handler for test with 100% droprate (There shouldn't be any messages coming through obviously).
 * @param msg Message to handle
 */
void TbusDatarateQueueTester::handleFullDrop(cMessage* msg) {
	// We don't expect any packets to come through
	success = false;
}

/**
 * Test a loss-free channel (0% droprate) with 10MBit/s datarate
 */
void TbusDatarateQueueTester::testNormalDatarate() {
	// Reset values
	queue->updateValue(NULL);
	queue->updateValue(NULL);

	TbusQueueDatarateValue* datarate = new TbusQueueDatarateValue();
	datarate->datarate = 10000000.0/8.0; //10Mbit
	datarate->droprate = 0.0;

//	datarateQueue->updateValue(datarate);
	queue->updateValue(datarate);

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

/**
 * Message handler for normal datarate.
 * @param msg Message to handle
 */
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

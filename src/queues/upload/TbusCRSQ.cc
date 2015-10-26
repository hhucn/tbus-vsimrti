//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusCRSQ.h"
#include "TbusCDSQ.h"

Define_Module(TbusCRSQ);

/**
 * Empty constructor.
 */
TbusCRSQ::TbusCRSQ() : TbusDatarateQueue(CRSQ) {}

void TbusCRSQ::initialize() {
	setQueueLength(par("queueLength").longValue());

	TbusDatarateQueue::initialize();
}

/**
 * If the queue is empty, inform CDSQ (the next queue in line) to start saving values.
 * @see TbusDatarateQueue::addPacketToQueue(cPacket*)
 * @param packet Packet to add
 */
bool TbusCRSQ::addPacketToQueue(cPacket* packet) {
	bool result = TbusDatarateQueue::addPacketToQueue(packet);

	// If packet was added and queue is empty, start collecting values in the next delay queue
	if (result && (queue.length() == 1)) {
		send(new cMessage(NULL, START_RECORDING), outGate);
	}

	return result;
}

/**
 * Informs CDSQ (the next queue in line) to reset its saved values and to stop saving values if this was the last packet in line for now.
 * @see TbusDatarateQueue::sendFrontOfQueue()
 */
void TbusCRSQ::sendFrontOfQueue() {
	TbusDatarateQueue::sendFrontOfQueue();

	// If queue is empty after sending, stop collecting values in the next delay queue
	if (queue.empty()) {
		send(new cMessage(NULL, STOP_RECORDING), outGate);
	}
}

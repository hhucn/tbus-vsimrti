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

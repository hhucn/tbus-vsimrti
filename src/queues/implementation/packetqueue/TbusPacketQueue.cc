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

#include "TbusPacketQueue.h"
#include "TbusQueueControlInfo.h"

/**
 * Empty constructor
 */
TbusPacketQueue::TbusPacketQueue(): queueLength(-1) {}

/**
 * Empty destructor
 */
TbusPacketQueue::~TbusPacketQueue() {}

/**
 * Extends cPacketQueue by setting queue arrival time on TbusQueueControlInfo
 * and calling cPacketQueue's insert
 * @see cPacketQueue::insert(cPacket*)
 * @param packet Packet to insert
 */
bool TbusPacketQueue::insertTbusPacket(cPacket* packet) {
	if ((queueLength == -1) || (length() < queueLength)) {
		TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->getControlInfo());

		// Set queue arrival time
		controlInfo->setQueueArrival(simTime());

		// Also set head of queue time if the queue is currently empty
		if (empty()) {
			controlInfo->setHeadOfQueue(simTime());
		}

		cPacketQueue::insert(packet);
		return true;
	} else {
		// Delete the packet
		delete packet;
	}

	return false;
}

/**
 * Extends cPacketQueue by updating head of queue time for the next head of queue (if any)
 * @see cPacketQueue::pop()
 * @return Head of Queue packet
 */
cPacket* TbusPacketQueue::pop() {
	cPacket* packet = cPacketQueue::pop();

	if (!empty()) {
		TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(front()->getControlInfo());
		// Set head of queue time
		controlInfo->setHeadOfQueue(simTime());
	}

	return packet;
}

void TbusPacketQueue::setQueueLength(int64_t queueLength) {
	this->queueLength = queueLength;
}

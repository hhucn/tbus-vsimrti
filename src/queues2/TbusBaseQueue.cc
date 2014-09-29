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

#include "TbusBaseQueue.h"
#include "omnetpp.h"

/**
 * Set up.
 */
template<class T> TbusBaseQueue<T>::TbusBaseQueue() :
	queue(cPacketQueue()),
	selfMessage(cMessage(TBUS_BASE_QUEUE_SELFMESSAGE)),
	values(std::vector<T>()) {
}

/**
 * Clean up.
 */
template<class T> TbusBaseQueue<T>::~TbusBaseQueue() {
	values.clear();
	queue.clear();
	delete currentValue;
}

/**
 * Sets gate indices.
 */
template<class T> void TbusBaseQueue<T>::initialize() {
	inGate = findGate("inGate");
	outGate = findGate("outGate");
}

/**
 * Handles the message accordingly if it is a self message or a packet.
 * @param msg message to handle
 */
template<class T> void TbusBaseQueue<T>::handleMessage(cMessage* msg) {
	if (msg->isSelfMessage()) {
		this->handleSelfMessage(msg);
	} else {
		cPacket* packet = check_and_cast<cPacket*>(msg);

		if (packet) {
			this->addPacketToQueue(packet);
		} else {
			opp_error("received invalid message - only self messages or packets accepted!");
			delete msg;
		}
	}
}

/**
 * Sends the front of queue and inspects the next packets' earliest delivery time, schedules accordingly.
 * @param msg self message
 */
template<class T> void TbusBaseQueue<T>::handleSelfMessage(cMessage* msg) {
	if (strcmp(msg->getName(), TBUS_BASE_QUEUE_SELFMESSAGE) == 0) {
		// First, send the front packet
		this->sendFrontOfQueue();

		// Then check the next one and/or reschedule
		if (queue.length() > 0) {
			TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(queue.front()->getControlInfo());
			ASSERT2(controlInfo, "Invalid control info on packet!");

			if (controlInfo->getEarliestDelivery() <= simTime()) {
				scheduleAt(simTime(), &selfMessage);
			} else {
				scheduleAt(controlInfo->getEarliestDelivery(), &selfMessage);
			}
		}
	} else {
		opp_error("Received invalid self message!");
	}
}

/**
 * Adds a TbusQueueControlInfo to the packet and inserts it into the queue, then starts the sending process (if there is none).
 * @param packet packet to add
 */
template <class T> void TbusBaseQueue<T>::addPacketToQueue(cPacket* packet) {
	take(packet);
	packet->setControlInfo(new TbusQueueControlInfo());

	this->calculateEarliestDeliveryForPacket(packet);
	queue.insert(packet);

	if (!selfMessage.isScheduled()) {
		scheduleAt(((TbusQueueControlInfo*) packet->getControlInfo())->getEarliestDelivery(), &selfMessage);
	}
}

/**
 * Cancels the current self message (if any), recalculates earliest deliveries and schedules a new self message.
 */
template<class T> void TbusBaseQueue<T>::adaptSelfMessage() {
	ASSERT2(queue.length() > 0, "Queue has to have length > 0!");

	if (selfMessage.isScheduled()) {
		cancelEvent(&selfMessage);
	}

	this->calculateEarliestDeliveries();

	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(queue.front()->getControlInfo());
	ASSERT2(controlInfo, "Invalid control info on packet!");

	scheduleAt(controlInfo->getEarliestDelivery(), &selfMessage);
}

/**
 * Removes the front packet from the queue and sends it to the out gate.
 * Also clears all but the current value.
 */
template<class T> void TbusBaseQueue<T>::sendFrontOfQueue() {
	ASSERT2(queue.length() > 0, "Queue has to have length > 0!");
	cPacket* packet = queue.pop();

	TbusQueueControlInfo* controlInfo = packet->removeControlInfo();
	ASSERT2(controlInfo->getEarliestDelivery() <= simTime(), "Sending packet earlier than expected!");
	delete controlInfo;

	send(packet, outGate);
	drop(packet);

	values.clear();
	values.push_back(currentValue);
}

/**
 * Adds the new value to the list if there are ongoing transmissions or uses it as the new value if there are none.
 * @param newValue value to update to
 */
template<class T> void TbusBaseQueue<T>::updateValue(T* newValue) {
	if (currentValue != newValue) {
		// Clear old values
		if (queue.isEmpty()) {
			values.clear();
		}

		// Store new value
		values.push_back(newValue);
		currentValue = newValue;

		// If there are ongoing transmissions, update their earliest deliveries
		if (!queue.isEmpty()) {
			this->adaptSelfMessage();
		}
	}
}

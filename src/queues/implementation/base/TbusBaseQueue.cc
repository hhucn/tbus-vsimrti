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
#include "TbusQueueDatarateValue.h"
#include "TbusQueueDelayValue.h"
#include "omnetpp.h"

/**
 * Set up.
 * Instantiates selfMessage with name #TBUS_BASE_QUEUE_SELFMESSAGE
 */
template<class T> TbusBaseQueue<T>::TbusBaseQueue() : selfMessage(TBUS_BASE_QUEUE_SELFMESSAGE, 0) {}

/**
 * Clean up.
 */
template<class T> TbusBaseQueue<T>::~TbusBaseQueue() {
	queue.clear();
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
 * @param msg Message to handle
 */
template<class T> void TbusBaseQueue<T>::handleMessage(cMessage* msg) {
	if (msg->isSelfMessage()) {
		this->handleSelfMessage(msg);
	} else {
		cPacket* packet = check_and_cast<cPacket*>(msg);

		if (packet) {
			this->addPacketToQueue(packet);
		} else {
			throw cRuntimeError("received invalid message - only self messages or packets accepted!");
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

		//Only leave current value
		T* currentValue = new T(*values.front());
		clearAndDeleteValues();
		values.push_front(currentValue);

		// Then check the next one and/or reschedule
		if (queue.length() > 0) {
			// Re-calculate earliest deliveries
			calculateEarliestDeliveries();
		}
	} else {
		throw cRuntimeError("Received invalid self message!");
	}
}

/**
 * Adds a packet to the queue, then starts the sending process (if there is none).
 * @param packet packet to add
 */
template <class T> void TbusBaseQueue<T>::addPacketToQueue(cPacket* packet) {
	// Update queue arrival time
	((TbusQueueControlInfo*) packet->getControlInfo())->setQueueArrival(simTime());
	queue.insert(packet);
	this->calculateEarliestDeliveryForPacket(packet);

	if (!selfMessage.isScheduled()) {
		adaptSelfMessage();
	}
}

/**
 * Cancels the current self message (if any), recalculates earliest deliveries and schedules a new self message.
 */
template<class T> void TbusBaseQueue<T>::adaptSelfMessage() {
	ASSERT2(queue.length() > 0, "Queue has to have length > 0!");
	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(queue.front()->getControlInfo());
	ASSERT2(controlInfo, "Invalid control info on packet!");

	if (selfMessage.getArrivalTime() != controlInfo->getEarliestDelivery()) {
		if (selfMessage.isScheduled()) {
			cancelEvent(&selfMessage);
		}

		// Take now or a future time as next schedule
		simtime_t nextSchedule = SimTime(std::max(simTime().inUnit(SIMTIME_NS), controlInfo->getEarliestDelivery().inUnit(SIMTIME_NS)), SIMTIME_NS);
		scheduleAt(nextSchedule, &selfMessage);
	}
}

/**
 * Removes the front packet from the queue and sends it to the out gate.
 * Also clears all but the current value.
 */
template<class T> void TbusBaseQueue<T>::sendFrontOfQueue() {
	cPacket* packet = getAndRemoveHeadOfQueue();

	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->getControlInfo());
	ASSERT2(controlInfo, "Invalid control info on packet!");
	ASSERT2(controlInfo->getEarliestDelivery() <= simTime(), "Sending packet earlier than expected!");

	EV << this->getName() << ": dispatching packet " << packet << " at " << simTime() << std::endl;

	send(packet, outGate);
}

template<class T> cPacket* TbusBaseQueue<T>::getAndRemoveHeadOfQueue() {
	ASSERT2(queue.length() > 0, "Queue has to have length > 0!");
	cPacket* packet = queue.pop();

	// Update head of queue time on next packet (if there is any)
	if (!queue.empty()) {
		((TbusQueueControlInfo*) queue.front()->getControlInfo())->setHeadOfQueue(simTime());
	}

	return packet;
}

/**
 * Adds the new value to the list if there are ongoing transmissions or uses it as the new value if there are none.
 * @param newValue value to update to
 */
template<class T> void TbusBaseQueue<T>::updateValue(T* newValue) {
	Enter_Method("updateValue()");
	if (values.empty() || values.front() != newValue) {
		// Clear old values
		if (queue.isEmpty()) {
			clearAndDeleteValues();
		}

		// Store new value
		values.push_front(newValue);

		// If there are ongoing transmissions, update their earliest deliveries
		if (!queue.isEmpty()) {
			calculateEarliestDeliveries();
		}
	} else {
		delete newValue;
	}
}

/**
 * Deletes all value objects and clears the deque
 */
template<class T> void TbusBaseQueue<T>::clearAndDeleteValues() {
	for (valueIterator it = values.begin(); it != values.end(); ++it) {
		delete *it;
	}
	values.clear();
}

/**
 * Called upon simulation end.
 * Releases selfmessage and cancels any future self message events.
 * Clears the queue of packets.
 */
template<class T> void TbusBaseQueue<T>::finish() {
		// Simulation Cleanup
		if (selfMessage.isScheduled()) {
			cancelEvent(&selfMessage);
		}

		clearAndDeleteValues();
		queue.clear();
}

// Template instantiation for linker
template class TbusBaseQueue<TbusQueueDatarateValue>;
template class TbusBaseQueue<TbusQueueDelayValue>;

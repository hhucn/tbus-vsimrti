//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusBaseQueue.h"
#include "TbusQueueDatarateValue.h"
#include "TbusQueueDelayValue.h"
#include "omnetpp.h"

#include <algorithm>

/**
 * Set up.
 * Instantiates selfMessage
 */
template<class T> TbusBaseQueue<T>::TbusBaseQueue(TbusQueueSelection selection) :
	selfMessage(NULL, 0),
	queueSelection(selection),
	queueStatus(INACTIVE) {}

/**
 * Empty destructor, clean up is done in finish().
 * @see TbusBaseQueue<T>::finish()
 */
template<class T> TbusBaseQueue<T>::~TbusBaseQueue() {}

/**
 * Sets gate indices.
 */
template<class T> void TbusBaseQueue<T>::initialize() {
	inGate = findGate("inGate");
	outGate = findGate("outGate");
}

template<class T> void TbusBaseQueue<T>::setQueueControlCallback(TbusQueueControlCallback* qcCallback) {
	callback = qcCallback;
}

/**
 * Sets the TBUS packet queue's queue length
 * @param queueLength Queue length
 */
template<class T> void TbusBaseQueue<T>::setQueueLength(int64_t queueLength) {
	queue.setQueueLength(queueLength);
}

/**
 * Set the status for value updates.
 * @param status Value updates status
 */
template<class T> void TbusBaseQueue<T>::setQueueStatus(TbusQueueStatus status) {
	if (status != queueStatus) {
		// Only update if different
		queueStatus = status;

		// Make queue control aware of change
		callback->queueStatusChanged(queueSelection);
	}
}

/**
 * Get this queues status.
 * @return Queue status
 */
template<class T> TbusQueueStatus TbusBaseQueue<T>::getQueueStatus() const {
	return queueStatus;
}

/**
 * Handles the message accordingly if it is a self message or a packet.
 * @param msg Message to handle
 */
template<class T> void TbusBaseQueue<T>::handleMessage(cMessage* msg) {
	if (msg->isSelfMessage()) {
		handleSelfMessage(msg);
	} else {
		cPacket* packet = check_and_cast<cPacket*>(msg);
		addPacketToQueue(packet);
	}
}

/**
 * Sends the front of queue and inspects the next packets' earliest delivery time, schedules accordingly.
 * @param msg self message
 */
template<class T> void TbusBaseQueue<T>::handleSelfMessage(cMessage* msg) {
	// First, send the front packet
	sendFrontOfQueue();

	//Only leave current value
	clearAndDeleteValues(TBUS_CLEAR_ALL_EXCEPT_FRONT);

	// Then check the next one and/or reschedule
	if (!queue.isEmpty()) {
		// Re-calculate earliest deliveries
		calculateEarliestDeliveries();
	}
}

/**
 * Adds a packet to the queue, then starts the sending process (if there is none).
 * @param packet packet to add
 */
template <class T> bool TbusBaseQueue<T>::addPacketToQueue(cPacket* packet) {
	bool result = queue.insertTbusPacket(packet);

	if (result) {
		// Only act if packet could be inserted
		if (queue.length() == 1) {

			// First packet in queue, set this queue as active
			setQueueStatus(ACTIVE);
		}

		// Only calculate now when we have network characteristics
		if (!values.empty()) {
			calculateEarliestDeliveryForPacket(packet);

			if (!selfMessage.isScheduled()) {
				adaptSelfMessage();
			}
		}
	}

	return result;
}

/**
 * Cancels the current self message (if any), recalculates earliest deliveries and schedules a new self message.
 */
template<class T> void TbusBaseQueue<T>::adaptSelfMessage() {
	ASSERT2(queue.length() > 0, "Queue has to have length > 0!");
	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(queue.front()->getControlInfo());

	if (selfMessage.isScheduled()) {
		cancelEvent(&selfMessage);
	}

	// Take now or a future time as next schedule
	simtime_t nextSchedule = SimTime(std::max(simTime().inUnit(SIMTIME_NS), controlInfo->getEarliestDelivery().inUnit(SIMTIME_NS)), SIMTIME_NS);
	scheduleAt(nextSchedule, &selfMessage);
}

/**
 * Removes the front packet from the queue and sends it to the out gate.
 * Also clears all but the current value.
 */
template<class T> void TbusBaseQueue<T>::sendFrontOfQueue() {
	cPacket* packet = queue.pop();

	if (queue.empty()) {
		// Last packet in queue, set queue inactive
		setQueueStatus(INACTIVE);
	}

	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->getControlInfo());
	ASSERT2(controlInfo->getEarliestDelivery() <= simTime(), "Sending packet earlier than expected!");

	EV << this->getName() << ": dispatching packet " << packet << " at " << simTime() << std::endl;
#ifdef TBUS_DEBUG
	std::cout << simTime() << " - " << this->getName() << ": " << packet << " sent after " << (simTime() - controlInfo->getQueueArrival()) << ", added at " << controlInfo->getQueueArrival() << endl;
#endif /* TBUS_DEBUG */
	send(packet, outGate);
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
 * Templated function used for value deletion in std::for_each
 * @param value Value to delete
 */
template<class T> void deleteValue(T* value) {
		delete value;
}

/**
 * Deletes all value objects and clears the deque
 */
template<class T> void TbusBaseQueue<T>::clearAndDeleteValues(const TbusClearMethod method) {
	T* firstValue;
	if (method == TBUS_CLEAR_ALL_EXCEPT_FRONT) {
		firstValue = new T(*(values.front()));
	}

	// Remove all values and clear the values
	std::for_each(values.begin(), values.end(), deleteValue<T>);
	values.clear();

	if (method == TBUS_CLEAR_ALL_EXCEPT_FRONT) {
		values.push_front(firstValue);
	}
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

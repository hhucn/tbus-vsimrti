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

#include "TbusCDSQ.h"

Define_Module(TbusCDSQ);

/**
 * Start without saving values
 */
TbusCDSQ::TbusCDSQ() : TbusDelayQueue(CDSQ) {
	// Resize to one for direct assignment
	values.resize(1);
}

/**
 * Update/store the new value according to the saveValues flag.
 * If it is true, add the value to the store, if it is false, replace the only current value with the new value.
 * Also handles memory cleanup of duplicate and replaced values.
 * @see #saveValues
 * @param newValue New value to use.
 */
void TbusCDSQ::updateValue(TbusQueueDelayValue* newValue) {
	EV << "New CDSQ delay value "<< newValue->delay << endl;
	if (queueStatus && (values.empty() || values.front() != newValue)) {
		// Store new value
		values.push_front(newValue);

		if (!queue.isEmpty()) {
			calculateEarliestDeliveries();
		}
	} else if (!queueStatus) {
		// Replace existing value
		delete values[0];
		values[0] = newValue;

		if (!queue.isEmpty()) {
			calculateEarliestDeliveries();
		}
	} else {
		delete newValue;
	}
}

/**
 * Calls the base queues add method and clears all values except the current
 * @param packet Packet to send
 */
void TbusCDSQ::addPacketToQueue(cPacket* packet) {
	TbusDelayQueue::addPacketToQueue(packet);

	// Clear and delete all values but the front
	clearAndDeleteValues(TBUS_CLEAR_ALL_EXCEPT_FRONT);
	// Set a new saveTime
	saveTime = simTime().inUnit(SIMTIME_NS);
}

/**
 * Empty method body because calculation is done on packet arrival.
 */
void TbusCDSQ::calculateEarliestDeliveries() {
	// We don't. Calculations are done on packet arrival.
}

/**
 * Checks for control messages and acts accordingly. If msg is no control message, call the base class's handleMessage.
 * @see #TBUS_DELAY_QUEUE_START_SAVE_VALUES
 * @see #TBUS_DELAY_QUEUE_STOP_SAVE_VALUES
 * @param msg Message to handle.
 */
void TbusCDSQ::handleMessage(cMessage* msg) {
	std::cout << simTime() << " - " <<  this->getName() << ": Received message of kind " << msg->getKind() << endl;
	if (msg->getKind() == START_RECORDING) {
		EV << this->getName() << ": Start saving values at " << simTime() << std::endl;
		delete msg;

		setQueueStatus(ACTIVE);
		saveTime = simTime().inUnit(SIMTIME_NS);
	} else if (msg->getKind() == STOP_RECORDING) {
		EV << this->getName() << ": Stop saving values at " << simTime() << std::endl;
		delete msg;

		setQueueStatus(INACTIVE);
	} else {
		TbusDelayQueue::handleMessage(msg);
	}
}

/**
 * Handles a self message.
 * @param msg Self message to handle.
 */
void TbusCDSQ::handleSelfMessage(cMessage* msg) {
	// First, send the front packet
	sendFrontOfQueue();

	// Then check the next one and/or reschedule
	if (queue.length() > 0) {
		// Adapt self message for next head of queue
		adaptSelfMessage();
	}
}

/**
 * Calculates the current weighted delay since saveTime
 * @return Current weighted delay
 */
simtime_t TbusCDSQ::currentDelay() {
	ASSERT2(!values.empty(), "Empty values array!");
	// Calculations adapted from Tobias Krauthoffs work

	int64_t now = simTime().inUnit(SIMTIME_NS);

	int64_t delay = 0;
	int64_t start;
	int64_t end = now;

	// We want to iterate up to the second-last element (if any)
	valueIterator endIterator = values.end();
	endIterator--;

	for (valueIterator it = values.begin(); it != endIterator; ++it) {
		start = (*it)->time;
		delay += (*it)->delay * (end - start);
		end = start;
	}

	// Different handling of the last value: start is now our saveTime
	start = saveTime;
	delay += values.back()->delay * (end - start);

	// Normalize delay
	delay /= (now - saveTime);

	return SimTime(delay, SIMTIME_NS);
}

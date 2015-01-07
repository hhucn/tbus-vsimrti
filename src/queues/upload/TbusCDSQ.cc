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

#include <TbusCDSQ.h>

Define_Module(TbusCDSQ);

/**
 * Start without saving values and set the value deques' size to 1 (This is needed to store at least one value by simple [0]-assignment).
 */
TbusCDSQ::TbusCDSQ() : TbusDelayQueue(), saveValues(false) {
	// One element to make value updating safe
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
	Enter_Method("updateValue()");
	if (saveValues && values.front() != newValue) {
		// Store new value
		values.push_front(newValue);

		EV << this->getName() << ": Updated value at " << simTime() << std::endl;
	} else if (!saveValues) {
		delete values[0];
		values[0] = newValue;

		EV << this->getName() << ": Updated value at " << simTime() << std::endl;
	} else {
		// Cleanup
		delete newValue;
	}
}

/**
 * Empty method body because calculation is done on packet arrival.
 */
void TbusCDSQ::calculateEarliestDeliveries() {
	// We don't. Calculations are done on packet arrival.
}

/**
 * Checks for control messages and acts accordingly. If msg is no control message, call the base class' handleMessage.
 * @see #TBUS_DELAY_QUEUE_START_SAVE_VALUES
 * @see #TBUS_DELAY_QUEUE_STOP_SAVE_VALUES
 * @see #TBUS_DELAY_QUEUE_RESET_VALUES
 * @param msg Message to handle.
 */
void TbusCDSQ::handleMessage(cMessage* msg) {
	if (strcmp(msg->getName(), TBUS_DELAY_QUEUE_START_SAVE_VALUES) == 0) {
		EV << this->getName() << ": Start saving values at " << simTime() << std::endl;
		delete msg;

		saveValues = true;
	} else if (strcmp(msg->getName(), TBUS_DELAY_QUEUE_STOP_SAVE_VALUES) == 0) {
		EV << this->getName() << ": Stop saving values at " << simTime() << std::endl;
		delete msg;

		saveValues = false;
	} else if (strcmp(msg->getName(), TBUS_DELAY_QUEUE_RESET_VALUES) == 0) {
		EV << this->getName() << ": Resetting saved values at " << simTime() << std::endl;
		delete msg;

		// Store a copy of the front value
		TbusQueueDelayValue* currentValue = new TbusQueueDelayValue(*(values.front()));
		// Delete all values before the packet entered the queue
		valueIterator it;
		for (it = values.begin(); it != values.end(); ++it) {
			if ((*it)->time >= simTime()) {
				// Value set after or with packet arrival, delete up to this value
				break;
			}
		}
		// Delete the values
		values.erase(values.begin(), it);

		// Add the saved value if our values array is empty
		if (values.empty()) {
			values.push_front(currentValue);
		} else {
			delete currentValue;
		}
	} else {
		TbusDelayQueue::handleMessage(msg);
	}
}

/**
 * Handles a #TBUS_BASE_QUEUE_SELFMESSAGE, throws an error upon receiving other self messages.
 * @see #TBUS_BASE_QUEUE_SELFMESSAGE
 * @param msg Self message to handle.
 */
void TbusCDSQ::handleSelfMessage(cMessage* msg) {
	if (strcmp(msg->getName(), TBUS_BASE_QUEUE_SELFMESSAGE) == 0) {
		// First, send the front packet
		this->sendFrontOfQueue();

		// Then check the next one and/or reschedule
		if (queue.length() > 0) {
			// Re-calculate earliest deliveries
			this->adaptSelfMessage();
		}
	} else {
		throw cRuntimeError("Received invalid self message!");
	}
}

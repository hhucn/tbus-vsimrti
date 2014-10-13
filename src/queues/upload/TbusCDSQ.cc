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

TbusCDSQ::TbusCDSQ() : TbusDelayQueue(), saveValues(false) {
	// One element to make value updating safe
	values.resize(1);
}

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

void TbusCDSQ::calculateEarliestDeliveries() {
	// We don't. Calculations are done on packet arrival.
}

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
		opp_error("Received invalid self message!");
	}
}

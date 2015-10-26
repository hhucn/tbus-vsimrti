//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusCDRQ.h"

Define_Module(TbusCDRQ);

/**
 * Empty constructor.
 */
TbusCDRQ::TbusCDRQ() : TbusDelayQueue(CDRQ) {}

void TbusCDRQ::initialize() {
	setQueueLength(par("queueLength").longValue());

	TbusDelayQueue::initialize();
}

/**
 * Replaces the current value with the new value (if different).
 * @param newValue Value to replace current value with
 */
void TbusCDRQ::updateValue(TbusQueueDelayValue* newValue) {
	Enter_Method("updateValue()");
	if (values.empty()) {
		values.push_back(newValue);
	} else if (values.front() != newValue) {
		// Store only new value
		delete values[0];
		values[0] = newValue;

		if (!queue.isEmpty()) {
			calculateEarliestDeliveries();
		}
	} else {
		delete newValue;
	}
}

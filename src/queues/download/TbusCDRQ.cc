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

#include "TbusCDRQ.h"

Define_Module(TbusCDRQ);

/**
 * Empty constructor.
 */
TbusCDRQ::TbusCDRQ() : TbusDelayQueue(CDRQ) {}

/**
 * Replaces the current value with the new value (if different).
 * @param newValue Value to replace current value with
 */
void TbusCDRQ::updateValue(TbusQueueDelayValue* newValue) {
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

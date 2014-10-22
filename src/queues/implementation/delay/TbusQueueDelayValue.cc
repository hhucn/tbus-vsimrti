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

#include "TbusQueueDelayValue.h"

/**
 * Empty constructor.
 */
TbusQueueDelayValue::TbusQueueDelayValue() : TbusQueueValue() {}

/**
 * Copy constructor.
 * @param other Object to copy values from (including creation time)
 */
TbusQueueDelayValue::TbusQueueDelayValue(const TbusQueueDelayValue& other) :
	delay(other.delay) {
	time = other.time;
}

/**
 * Comparison operator.
 * Returns true if delays are different, false otherwise.
 * Also returns true if other is not of type #TbusQueueDelayValue.
 * @param other Object to compare against
 * @return Comparison value of delay
 */
bool TbusQueueDelayValue::operator!=(TbusQueueValue& other) {
	TbusQueueDelayValue* cother = check_and_cast<TbusQueueDelayValue*>(&other);

	if (cother) {
		return (this->delay != cother->delay);
	} else {
		return true;
	}
}

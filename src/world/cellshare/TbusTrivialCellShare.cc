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

#include "TbusTrivialCellShare.h"
#include "TbusQueueDatarateValue.h"

TbusTrivialCellShare::TbusTrivialCellShare() {}

/**
 * Adapt datarate value for cell id.
 * @param cellId Cell id
 * @param value Value to adapt
 * @param host Host
 */
TbusQueueDatarateValue* TbusTrivialCellShare::adaptDatarateValue(cellid_t cellId, TbusQueueDatarateValue* value, TbusHost* host) {
	TbusQueueDatarateValue* newValue = new TbusQueueDatarateValue(*value);
	uint64_t activeHosts = getActiveHostsInCell(cellId);

	// We use an equal division plus 10% more bandwidth
	double_t factor = (activeHosts == 1) ? 1.0 : 1.1;
	factor /= (double_t) activeHosts;

	newValue->datarate *= factor;

	if (newValue->datarate <= 0) {
		std::cout << simTime() << " datarate " << newValue->datarate << " drop " << newValue->droprate << " cell " << cellId << " factor " << factor << " active " << activeHosts << std::endl;
	}

	return newValue;
}

/**
 * Adapt delay value for cell id.
 * @param cellId Cell id
 * @param value Value to adapt
 * @param host Host
 */
TbusQueueDelayValue* TbusTrivialCellShare::adaptDelayValue(cellid_t cellId, TbusQueueDelayValue* value, TbusHost* host) {
	TbusQueueDelayValue* newValue = new TbusQueueDelayValue(*value);

	return newValue;
}

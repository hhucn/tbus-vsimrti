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

TbusTrivialCellShare::~TbusTrivialCellShare() {
	cellToNumHosts.clear();
}

/**
 * Register host at cell.
 * @param cellId Cell id
 * @param host Host
 */
void TbusTrivialCellShare::registerHost(cellid_t cellId, cModule* host) {
	cellToNumHosts[cellId]++;
}

/**
 * Unregister host from cell.
 * @param cellId Cell id
 * @param host Host
 */
void TbusTrivialCellShare::unregisterHost(cellid_t cellId, cModule* host) {
	ASSERT2(cellToNumHosts[cellId] > 0, "Invalid unregister, no hosts registered to this cell!");

	cellToNumHosts[cellId]--;
}

/**
 * Adapt value for cell id. Only adapts TbusQueueDatarateValues.
 * @param cellId Cell id
 * @param value Value to adapt
 * @param host Host
 */
void TbusTrivialCellShare::adaptValue(cellid_t cellId, TbusQueueValue* value, cModule* host) {
	ASSERT2(cellToNumHosts[cellId] > 0, "Invalid adaptValue, no hosts registered to this cell!");

	// We use an equal division plus 10% more bandwidth
	double_t factor = (cellToNumHosts[cellId] == 1) ? 1.0 : 1.1;
	factor /= cellToNumHosts[cellId];

	// Only adapt DatarateValues
	TbusQueueDatarateValue* datarateValue = dynamic_cast<TbusQueueDatarateValue*>(value);
	if (datarateValue) {
		datarateValue->datarate *= factor;
	}
}

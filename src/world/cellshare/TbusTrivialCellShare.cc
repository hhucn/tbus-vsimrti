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

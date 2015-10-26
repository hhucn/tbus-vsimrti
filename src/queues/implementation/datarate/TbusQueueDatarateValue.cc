//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusQueueDatarateValue.h"
#include "TbusSqliteDatabaseHandler.h"

/**
 * Empty constructor.
 */
TbusQueueDatarateValue::TbusQueueDatarateValue() : TbusQueueValue() {}

/**
 * Copy constructor.
 * Set data- and droprate to other's values and time to current simulation time
 * @param other Object to copy values from
 */
TbusQueueDatarateValue::TbusQueueDatarateValue(const TbusQueueDatarateValue& other) :
	droprate(other.droprate),
	datarate(other.datarate) {
	time = simTime().inUnit(SIMTIME_NS);
}

/**
 * Comparison operator.
 * Returns true if data- and/or droprate of both objects are different, false otherwise.
 * Also returns true if other is not of type #TbusQueueDatarateValue
 * @param other The object to compare against
 * @return Comparison value of data- and droprate
 */
bool TbusQueueDatarateValue::operator!=(TbusQueueValue& other) {
	TbusQueueDatarateValue* cother = check_and_cast<TbusQueueDatarateValue*>(&other);

	if (cother) {
		return (droprate != cother->droprate) || (datarate != cother->datarate);
	} else {
		return true;
	}
}

bool TbusQueueDatarateValue::isValid() {
	return (datarate != TBUS_DATARATE_DEFAULT);
}

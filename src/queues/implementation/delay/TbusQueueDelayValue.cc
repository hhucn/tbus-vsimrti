//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusQueueDelayValue.h"
#include "TbusSqliteDatabaseHandler.h"

/**
 * Empty constructor.
 */
TbusQueueDelayValue::TbusQueueDelayValue() : TbusQueueValue() {}

/**
 * Copy constructor.
 * Sets delay to other delay and time to current simulation time.
 * @param other Object to copy values from
 */
TbusQueueDelayValue::TbusQueueDelayValue(const TbusQueueDelayValue& other) :
	delay(other.delay) {
	time = simTime().inUnit(SIMTIME_NS);
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
		return (delay != cother->delay);
	} else {
		return true;
	}
}

/**
 * Value state
 * @return Value state
 */
bool TbusQueueDelayValue::isValid() {
	return (delay != TBUS_DELAY_DEFAULT);
}

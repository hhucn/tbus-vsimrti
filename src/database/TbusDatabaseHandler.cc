/*
 * DatabaseHandler.cc
 *
 *  Created on: 16.07.2015
 *      Author: bialon
 */

#include "TbusDatabaseHandler.h"

Register_GlobalConfigOption(CFGID_TBUS_OFFSET, "tbus-offset", CFG_INT, 0, "Timestamp offset used for value retrieval")

TbusDatabaseHandler::TbusDatabaseHandler() {
	offset = SimTime(ev.getConfig()->getAsInt(CFGID_TBUS_OFFSET, 0), SIMTIME_NS);
};

/**
 * Returns the given simulation time (or now, if none) plus offset
 * @param time Simulation time
 * @return Time plus offset
 */
simtime_t TbusDatabaseHandler::getSimulationTimeWithOffset(simtime_t time) {
	return time + offset;
}

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

#include "TbusQueueControl.h"
#include "TbusSqliteDatabaseHandler.h"
#include "ModuleAccess.h"
#include "TbusQueueDatarateValue.h"
#include "TbusQueueDelayValue.h"
#include "TbusTrivialCellShare.h"
#include "TbusMobileNode.h"

#ifdef TBUS_QUEUE_TESTING
#include <sqlite3.h>
#endif

Define_Module(TbusQueueControl);

/**
 * Empty constructor.
 */
TbusQueueControl::TbusQueueControl() :
		dbHandler(TbusDatabaseHandler::getInstance<TbusSqliteDatabaseHandler>()),
		cellShare(TbusCellShare::getInstance<TbusTrivialCellShare>()),
		currentRoadId(NULL),
		online(false) {
}

/**
 * Get references to the modules' queues and the global database handler and coordinate converter.
 */
void TbusQueueControl::initialize() {
	// Get references to "our" queues
	cdrq = ModuleAccess<TbusCDRQ>("cdrq").get();
	crrq = ModuleAccess<TbusCRRQ>("crrq").get();
	crsq = ModuleAccess<TbusCRSQ>("crsq").get();
	cdsq = ModuleAccess<TbusCDSQ>("cdsq").get();

	converter = TbusCoordinateConverter::getInstance();

	currentCellId = TBUS_INVALID_CELLID;

	tbusHost.callback = this;
	tbusHost.host = this->getParentModule();

	// Find parent TbusMobileNode module
	while (!dynamic_cast<TbusMobileNode*>(tbusHost.host)) {
		tbusHost.host = tbusHost.host->getParentModule();
	}

#ifdef TBUS_QUEUE_TESTING
	// TESTING with selected timed updates from the database
	sqlite3* database;
	sqlite3_open_v2("test_edge.sqlite", &database, SQLITE_OPEN_READONLY, NULL);

	sqlite3_stmt* timestampSelect;
	sqlite3_prepare_v2(database, "select timestamp from upload_delay union select timestamp from upload_datarate union select timestamp from download_delay union select timestamp from download_datarate;", -1, &timestampSelect, NULL);

	cMessage* selfMessage;
	SimTime time;
	SimTime now = simTime();

	while (sqlite3_step(timestampSelect) == SQLITE_ROW) {
		// Add self messages
		selfMessage = new cMessage("timestampmessage", 0);
		time = SimTime(sqlite3_column_int64(timestampSelect, 0), SIMTIME_NS);

		if (time >= now) {
			scheduleAt(time, selfMessage);
		}
	}

	sqlite3_finalize(timestampSelect);
	sqlite3_close(database);
#endif
}

/**
 * Takes node out of the cell model.
 */
void TbusQueueControl::finish() {
	cellShare->hostMoved(currentCellId, TBUS_INVALID_CELLID, &tbusHost);
}

/**
 * Translates the given coordinates into latitute/longitude.
 * Then retrieves the belonging datarate, droprate and delay values from the database Handler.
 * Then the new values are updated on the belonging queue.
 * @param newCoords The new node position
 * @deprecated Use edge and position instead
 */
void TbusQueueControl::updateQueues(const Coord& newCoords) {
	// Act like this is part of our PHY layer
	Enter_Method_Silent();

	Coord translated = converter->translate(&newCoords);

	EV << "TbusQueueControl updating queues for coordinates " << newCoords << " (" << translated << ")\n";

	cdrq->updateValue(dbHandler->getDownloadDelay(translated));
	crrq->updateValue(dbHandler->getDownloadDatarate(translated));
	cdsq->updateValue(dbHandler->getUploadDelay(translated));
	crsq->updateValue(dbHandler->getUploadDatarate(translated));
}

/**
 * Retrieves delay, data- and droprate from the database handler for the place given by roadId and lanePos.
 * The new values are updated on the belonging queue.
 * @param roadId The new edge to look at
 * @param lanePos The new position on the edge to look at
 */
void TbusQueueControl::updateQueues(const char* const roadId, const float lanePos) {
	// Act like this is part of out PHY layer
	Enter_Method_Silent();

	EV << "TbusQueueControl updating queues for road id " << roadId  << " and lane position " << lanePos << "\n";
	std::cout << "TbusQueueControl updating queues for road id " << roadId  << " and lane position " << lanePos << "\n";

	TbusQueueDelayValue* delayValue;
	TbusQueueDatarateValue* datarateValue;

	delayValue = dbHandler->getDownloadDelay(roadId, lanePos);
	cellShare->adaptValue(currentCellId, delayValue);
	cdrq->updateValue(delayValue);

	datarateValue = dbHandler->getDownloadDatarate(roadId, lanePos);
	cellShare->adaptValue(currentCellId, datarateValue);
	crrq->updateValue(datarateValue);

	delayValue = dbHandler->getUploadDelay(roadId, lanePos);
	cellShare->adaptValue(currentCellId, delayValue);
	cdsq->updateValue(delayValue);

	datarateValue = dbHandler->getUploadDatarate(roadId, lanePos);
	cellShare->adaptValue(currentCellId, datarateValue);
	crsq->updateValue(datarateValue);

	online = true;
}

/**
 * Update cell id and inform the cell share model if it has changed.
 * Also enters a callback hook into the cellshare model to be called if every node has updated its cell id.
 * @param newRoadId New road id
 * @param newLanePos New lane position
 */
void TbusQueueControl::updateCellId(const char* const newRoadId, const float newLanePos) {
	// Update cell id, compare against old cell id and move
	cellid_t newCellId = dbHandler->getCellId(newRoadId, newLanePos);

	if (newCellId != currentCellId) {
		// Set new cell id for callback
		cellid_t tempCellId = currentCellId;
		currentCellId = newCellId;

		cellShare->hostMoved(tempCellId, newCellId, &tbusHost);
	} else {
		cellShare->hostMoved(TBUS_INVALID_CELLID, TBUS_INVALID_CELLID, &tbusHost);
	}
}

/**
 * Node position update notification method.
 * @param newRoadId New road id
 * @param newLanePos New lane position
 */
void TbusQueueControl::nodeMoved(const char* const newRoadId, const float newLanePos) {
	delete currentRoadId;
	currentRoadId = new char[strlen(newRoadId) + 1];
	strcpy(currentRoadId, newRoadId);

	currentLanePos = newLanePos;

	updateCellId(currentRoadId, currentLanePos);
}

/**
 * Update selected queue values if these queues are active
 * @param selection And-ded Queue types
 */
void TbusQueueControl::triggerQueueValueUpdate(TbusQueueSelection selection) {
	TbusQueueDelayValue* delayValue;
	TbusQueueDatarateValue* datarateValue;

	if ((selection && CDRQ) && cdrq->isActive()) {
		delayValue = dbHandler->getDownloadDelay(currentRoadId, currentLanePos);
		cellShare->adaptValue(currentCellId, delayValue);
		cdrq->updateValue(delayValue);
	}

	if ((selection && CRRQ) && crrq->isActive()) {
		datarateValue = dbHandler->getDownloadDatarate(currentRoadId, currentLanePos);
		cellShare->adaptValue(currentCellId, datarateValue);
		crrq->updateValue(datarateValue);
	}

	if ((selection && CDSQ) && cdsq->isActive()) {
		delayValue = dbHandler->getUploadDelay(currentRoadId, currentLanePos);
		cellShare->adaptValue(currentCellId, delayValue);
		cdsq->updateValue(delayValue);
	}

	if ((selection && CRSQ) && crsq->isActive()) {
		datarateValue = dbHandler->getUploadDatarate(currentRoadId, currentLanePos);
		cellShare->adaptValue(currentCellId, datarateValue);
		crsq->updateValue(datarateValue);
	}
}

/**
 * Are these queues online, e.g. do they already have queue values?
 * @return online status of queues
 */
bool TbusQueueControl::isOnline() const {
	return online;
}

/**
 * Is any queue active?
 * @return Active state
 */
bool TbusQueueControl::isActive() const {
	bool active = false;

	active |= cdrq->isActive();
	active |= crrq->isActive();
	active |= cdsq->isActive();
	active |= crsq->isActive();

	return active;
}

#ifdef TBUS_QUEUE_TESTING
void TbusQueueControl::handleMessage(cMessage* msg) {
	if (msg->isSelfMessage()) {
//		updateQueues("", 0.0);
		nodeMoved("roadId", 0.0);
	}

	delete msg;
}
#endif

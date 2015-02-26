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
		currentRoadId(NULL) {}

/**
 * Get references to the modules' queues and the global database handler and coordinate converter.
 */
void TbusQueueControl::initialize() {
	// Get references to "our" queues
	cdrq = ModuleAccess<TbusCDRQ>("cdrq").get();
	crrq = ModuleAccess<TbusCRRQ>("crrq").get();
	crsq = ModuleAccess<TbusCRSQ>("crsq").get();
	cdsq = ModuleAccess<TbusCDSQ>("cdsq").get();

	cdrq->setQueueControlCallback(this);
	crrq->setQueueControlCallback(this);
	crsq->setQueueControlCallback(this);
	cdsq->setQueueControlCallback(this);

	cdrqValue = NULL;
	crrqValue = NULL;
	crsqValue = NULL;
	cdsqValue = NULL;

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
	// Move host out of cellshare model
	cellShare->hostMoved(currentCellId, TBUS_INVALID_CELLID, &tbusHost);
}

/**
 * Update cell id from database.
 */
void TbusQueueControl::updateCellIdFromDatabase() {
	// Update cell id, compare against old cell id and move
	cellid_t newCellId = dbHandler->getCellId(currentRoadId, currentLanePos);

	if (newCellId != currentCellId) {
		cellShare->hostMoved(currentCellId, newCellId, &tbusHost);

		currentCellId = newCellId;
	}
}

/**
 * Update queue values for active queues in a given queue selection.
 * This does NOT update the active queues with adapted values!
 * @param selection Queue selection
 */
void TbusQueueControl::updateQueueValuesFromDatabase(TbusQueueSelection selection) {
	if ((selection & CDRQ) && cdrq->getQueueStatus()) {
		cdrqValue = dbHandler->getDownloadDelay(currentRoadId, currentLanePos);
	}

	if ((selection & CRRQ) && crrq->getQueueStatus()) {
		crrqValue = dbHandler->getDownloadDatarate(currentRoadId, currentLanePos);
	}

	if ((selection & CDSQ) && cdsq->getQueueStatus()) {
		cdsqValue = dbHandler->getUploadDelay(currentRoadId, currentLanePos);
	}

	if ((selection & CRSQ) && crsq->getQueueStatus()) {
		crsqValue = dbHandler->getUploadDatarate(currentRoadId, currentLanePos);
	}
}

/**
 * Set adapted queue values for active queues in a given queue selection.
 * @param selection Queue selection
 */
void TbusQueueControl::adaptQueueValues(TbusQueueSelection selection) {
	if ((selection & CDRQ) && cdrq->getQueueStatus()) {
		cdrq->updateValue(cellShare->adaptDelayValue(currentCellId, cdrqValue, &tbusHost));
	}

	if ((selection & CRRQ) && crrq->getQueueStatus()) {
		crrq->updateValue(cellShare->adaptDatarateValue(currentCellId, crrqValue, &tbusHost));
	}

	if ((selection & CDSQ) && cdsq->getQueueStatus()) {
		cdsq->updateValue(cellShare->adaptDelayValue(currentCellId, cdsqValue, &tbusHost));
	}

	if ((selection & CRSQ) && crsq->getQueueStatus()) {
		crsq->updateValue(cellShare->adaptDatarateValue(currentCellId, crsqValue, &tbusHost));
	}
}

/**
 * Updates currentRoadId and stores a copy.
 * @param roadId New road id
 */
void TbusQueueControl::setRoadId(const char* roadId) {
	if (currentRoadId != NULL) {
		delete[] currentRoadId;
	}

	currentRoadId = new char[strlen(roadId) + 1];
	strcpy(currentRoadId, roadId);
}

/**
 * Updates lanePos.
 * @param lanePos New lane position
 */
void TbusQueueControl::setLanePos(float lanePos) {
	currentLanePos = lanePos;
}

/**
 * Callback method for a queue status change.
 * Makes the cellshare model aware of that change.
 */
void TbusQueueControl::queueStatusChanged(TbusQueueSelection selection) {
	// Update the selected queue values from database
	updateQueueValuesFromDatabase(selection);

	// Make cellshare model aware of that change
	cellShare->cellActivityChanged(currentCellId);
}

/**
 * Get the highest queue status.
 * @return Highest queue status
 */
TbusQueueStatus TbusQueueControl::getQueueStatus() const {
	return (TbusQueueStatus) (cdrq->getQueueStatus() | crrq->getQueueStatus() | cdsq->getQueueStatus() | crsq->getQueueStatus());
}

#ifdef TBUS_QUEUE_TESTING
void TbusQueueControl::handleMessage(cMessage* msg) {
	if (msg->isSelfMessage()) {
//		nodeMoved("roadId", 0.0);
	}

	delete msg;
}
#endif

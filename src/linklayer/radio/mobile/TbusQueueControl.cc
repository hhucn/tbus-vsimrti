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

Define_Module(TbusQueueControl);

/**
 * Empty constructor.
 */
TbusQueueControl::TbusQueueControl() :
		dbHandler(TbusDatabaseHandler::getInstance<TbusSqliteDatabaseHandler>()),
		cellShare(TbusCellShare::getInstance<TbusTrivialCellShare>()),
		currentRoadId(NULL),
		roadIdValid(false) {
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

	cdrq->setQueueControlCallback(this);
	crrq->setQueueControlCallback(this);
	crsq->setQueueControlCallback(this);
	cdsq->setQueueControlCallback(this);

	cdrqValue = NULL;
	crrqValue = NULL;
	crsqValue = NULL;
	cdsqValue = NULL;

	// Set to invalid because this node hasn't moved into a cell yet
	currentCellId = TBUS_INVALID_CELLID;

	tbusHost.callback = this;
	tbusHost.host = this->getParentModule();

	// Find parent TbusMobileNode module
	while (!dynamic_cast<TbusMobileNode*>(tbusHost.host)) {
		tbusHost.host = tbusHost.host->getParentModule();
	}
}

/**
 * Delete previously assigned values.
 */
TbusQueueControl::~TbusQueueControl() {
	delete cdrqValue;
	delete crrqValue;
	delete crsqValue;
	delete cdsqValue;

	delete[] currentRoadId;
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
 * Informs cellShare of cell movement.
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
	// Only update selected and active queues with valid values
	if ((selection & CDRQ) && cdrq->getQueueStatus()) {
		TbusQueueDelayValue* temp = dbHandler->getDownloadDelay(currentRoadId, currentLanePos);

		if (temp->isValid()) {
			delete cdrqValue;
			cdrqValue = temp;
		} else {
			delete temp;
		}
	}

	if ((selection & CRRQ) && crrq->getQueueStatus()) {
		TbusQueueDatarateValue* temp = dbHandler->getDownloadDatarate(currentRoadId, currentLanePos);

		if (temp->isValid()) {
			delete crrqValue;
			crrqValue = temp;
		} else {
			delete temp;
		}
	}

	if ((selection & CDSQ) && cdsq->getQueueStatus()) {
		TbusQueueDelayValue* temp = dbHandler->getUploadDelay(currentRoadId, currentLanePos);

		if (temp->isValid()) {
			delete cdsqValue;
			cdsqValue = temp;
		} else {
			delete temp;
		}
	}

	if ((selection & CRSQ) && crsq->getQueueStatus()) {
		TbusQueueDatarateValue* temp = dbHandler->getUploadDatarate(currentRoadId, currentLanePos);

		if (temp->isValid()) {
			delete crsqValue;
			crsqValue = temp;
		} else {
			delete temp;
		}
	}
}

/**
 * Set adapted queue values for active queues in a given queue selection.
 * @param selection Queue selection
 */
void TbusQueueControl::adaptQueueValues(TbusQueueSelection selection) {
	if ((cdrqValue != NULL) && (selection & CDRQ) && cdrq->getQueueStatus()) {
		cdrq->updateValue(cellShare->adaptDelayValue(currentCellId, cdrqValue, &tbusHost));
	}

	if ((crrqValue != NULL) && (selection & CRRQ) && crrq->getQueueStatus()) {
		crrq->updateValue(cellShare->adaptDatarateValue(currentCellId, crrqValue, &tbusHost));
	}

	if ((cdsqValue != NULL) && (selection & CDSQ) && cdsq->getQueueStatus()) {
		cdsq->updateValue(cellShare->adaptDelayValue(currentCellId, cdsqValue, &tbusHost));
	}

	if ((crsqValue != NULL) && (selection & CRSQ) && crsq->getQueueStatus()) {
		crsq->updateValue(cellShare->adaptDatarateValue(currentCellId, crsqValue, &tbusHost));
	}
}

/**
 * Updates currentRoadId and stores a copy.
 * @param roadId New road id
 */
void TbusQueueControl::setRoadId(const char* roadId) {
	if (currentRoadId == NULL) {
		// The first road id we get is always accepted and seen as valid
		currentRoadId = new char[strlen(roadId) + 1];
		strcpy(currentRoadId, roadId);
		roadIdValid = true;
	} else {
		roadIdValid = (strcmp(roadId, "") != 0);
	}

	if (roadIdValid && strcmp(roadId, currentRoadId) != 0) {
		delete[] currentRoadId;
		currentRoadId = new char[strlen(roadId) + 1];
		strcpy(currentRoadId, roadId);
	}
}

/**
 * Updates lanePos.
 * @param lanePos New lane position
 */
void TbusQueueControl::setLanePos(float lanePos) {
	if (roadIdValid) {
		currentLanePos = lanePos;
	}
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

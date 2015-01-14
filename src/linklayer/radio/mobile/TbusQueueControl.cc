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


#include "SqliteDatabaseHandler.h"
#include "DatabaseHandler.h"
#include "TbusQueueControl.h"
#include "ModuleAccess.h"
#include "TbusQueueDatarateValue.h"
#include "TbusQueueDelayValue.h"

Define_Module(TbusQueueControl);

/**
 * Empty constructor.
 */
TbusQueueControl::TbusQueueControl() {}

/**
 * Get references to the modules' queues and the global database handler and coordinate converter.
 */
void TbusQueueControl::initialize() {
	// Get references to "our" queues
	cdrq = ModuleAccess<TbusCDRQ>("cdrq").get();
	crrq = ModuleAccess<TbusCRRQ>("crrq").get();
	crsq = ModuleAccess<TbusCRSQ>("crsq").get();
	cdsq = ModuleAccess<TbusCDSQ>("cdsq").get();

	dbHandler = DatabaseHandler::getInstance<SqliteDatabaseHandler>();
	converter = TbusCoordinateConverter::getInstance();
}

/**
 * Translates the given coordinates into latitute/longitude.
 * Then retrieves the belonging datarate, droprate and delay values from the database Handler.
 * Then the new values are updated on the belonging queue.
 *
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
 *
 * @param roadId The new edge to look at
 * @param lanePos The new position on the edge to look at
 */
void TbusQueueControl::updateQueues(const char* const roadId, float lanePos) {
	// Act like this is part of out PHY layer
	Enter_Method_Silent();

	EV << "TbusQueueControl updating queues for road id " << roadId  << " and lane position " << lanePos << "\n";

	cdrq->updateValue(dbHandler->getDownloadDelay(roadId, lanePos));
	crrq->updateValue(dbHandler->getDownloadDatarate(roadId, lanePos));
	cdsq->updateValue(dbHandler->getUploadDelay(roadId, lanePos));
	crsq->updateValue(dbHandler->getUploadDatarate(roadId, lanePos));
}

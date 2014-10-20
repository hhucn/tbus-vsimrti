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

TbusQueueControl::TbusQueueControl() {}

void TbusQueueControl::initialize() {
	// Get references to "our" queues
	cdrq = ModuleAccess<TbusCDRQ>("cdrq").get();
	crrq = ModuleAccess<TbusCRRQ>("crrq").get();
	crsq = ModuleAccess<TbusCRSQ>("crsq").get();
	cdsq = ModuleAccess<TbusCDSQ>("cdsq").get();

	dbHandler = DatabaseHandler::getInstance<SqliteDatabaseHandler>();
	converter = TbusCoordinateConverter::getInstance();
}

void TbusQueueControl::updateQueues(const Coord& newCoords) {
	// Act like this is part of our PHY layer
	Enter_Method_Silent();

	EV << "TbusQueueControl updating queues for coordinates " << newCoords << endl;

	TbusQueueDatarateValue* sendDatarate = dbHandler->getUploadDatarate(converter->translate(&newCoords));
	TbusQueueDatarateValue* receiveDatarate = dbHandler->getDownloadDatarate(converter->translate(&newCoords));
	TbusQueueDelayValue* sendDelay = dbHandler->getUploadDelay(converter->translate(&newCoords));
	TbusQueueDelayValue* receiveDelay = dbHandler->getUploadDelay(converter->translate(&newCoords));

	std::cout << "sda:" << sendDatarate->datarate << " rda:" << receiveDatarate->datarate << " sde:" << sendDelay->delay << " rde:" << receiveDelay->delay << std::endl;

	cdrq->updateValue(receiveDelay);
	crrq->updateValue(receiveDatarate);
	crsq->updateValue(sendDatarate);
	cdsq->updateValue(sendDelay);
}

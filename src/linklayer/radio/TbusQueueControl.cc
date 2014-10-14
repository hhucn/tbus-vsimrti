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

	DatabaseHandler* handler = DatabaseHandler::getInstance<SqliteDatabaseHandler>();
}

void TbusQueueControl::updateQueues(const Coord& newCoords) {
	// Act like this is part of our PHY layer
	Enter_Method_Silent();

	EV << "TbusQueueControl updating queues for coordinates " << newCoords << endl;

	TbusQueueDatarateValue* sendDatarate = new TbusQueueDatarateValue();
	TbusQueueDatarateValue* receiveDatarate = new TbusQueueDatarateValue();
	TbusQueueDelayValue* sendDelay = new TbusQueueDelayValue();
	TbusQueueDelayValue* receiveDelay = new TbusQueueDelayValue();

	// TODO: Get Datarate and Delay for Position via database/source

	sendDatarate->datarate = 10000000.0/8.0;
	receiveDatarate->datarate = 10000000.0/8.0;
	sendDatarate->droprate = 0.05;
	receiveDatarate->droprate = 0.05;

	sendDelay->delay = 0.0001;
	receiveDelay->delay = 0.0001;

	cdrq->updateValue(receiveDelay);
	crrq->updateValue(receiveDatarate);
	crsq->updateValue(sendDatarate);
	cdsq->updateValue(sendDelay);
}

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

#include <TbusQueueControl.h>
#include "ModuleAccess.h"
#include "Datarate.h"
#include "Bbdelay.h"

Define_Module(TbusQueueControl);

TbusQueueControl::TbusQueueControl()
{
	// TODO Auto-generated constructor stub

}

TbusQueueControl::~TbusQueueControl()
{
	// TODO Auto-generated destructor stub
}

void TbusQueueControl::initialize() {
	// Gates are not needed because we communicate directly with the queues
	// Just to silent warnings about non-connected gates
	cdrqControl		= findGate("cdrqControl");
	crrqControl		= findGate("crrqControl");
	crsqControl		= findGate("cdsqControl");
	cdsqControl		= findGate("cdsqControl");

	// Get references to "our" queues
	cdrq = ModuleAccess<projekt::CDRQ>("cdrq").get();
	crrq = ModuleAccess<projekt::CRRQ>("crrq").get();
	crsq = ModuleAccess<projekt::CRSQ>("crsq").get();
	cdsq = ModuleAccess<projekt::CDSQ>("cdsq").get();
}

void TbusQueueControl::updateQueues(const Coord& newCoords) {
	// Act like this is part of our PHY layer
	Enter_Method_Silent();

	EV << "TbusQueueControl updating queues for coordinates " << newCoords << endl;

	projekt::Datarate* sendDatarate = new projekt::Datarate();
	projekt::Datarate* receiveDatarate = new projekt::Datarate();
	projekt::Bbdelay* sendDelay = new projekt::Bbdelay();
	projekt::Bbdelay* receiveDelay = new projekt::Bbdelay();

	// TODO: Get Datarate and Delay for Position via database/source

	sendDatarate->setDatarateSim(10.0);
	receiveDatarate->setDatarateSim(10.0);
	sendDatarate->setDroprate(0.05);
	receiveDatarate->setDroprate(0.05);

	sendDelay->setBbdelay(100000);
	receiveDelay->setBbdelay(100000);

	cdrq->bbDelayChanged(receiveDelay);
	crrq->datarateChanged(receiveDatarate);
	crsq->datarateChanged(sendDatarate);
	cdsq->bbDelayChanged(sendDelay);

	delete sendDatarate;
	delete receiveDatarate;
	delete sendDelay;
	delete receiveDelay;
}

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

#include <datarate/TbusDatarateQueue.h>
#include "TbusQueueControlInfo.h"

Define_Module(TbusDatarateQueue);

TbusDatarateQueue::TbusDatarateQueue() :
	bitsSent(0) {
}

TbusDatarateQueue::~TbusDatarateQueue() {
}

void TbusDatarateQueue::calculateEarliestDeliveries() {
	for (int i = 0; i < queue.length(); i++) {
		this->calculateEarliestDeliveryForPacket(queue.get(i));
	}
}

void TbusDatarateQueue::calculateEarliestDeliveryForPacket(cPacket* packet) {
	simtime_t delay;
	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->getControlInfo());
	ASSERT2(controlInfo, "Invalid control info on packet!");

	// Only calculate for the first packet, the others have to wait
	if (queue.front() == packet) {
		// Calculate earliest delivery with respect to already sent bytes

		// What time shall we use?
		simtime_t starttime = std::max(values.back()->time.dbl(), controlInfo->getQueueArrival().dbl());
		simtime_t runtime = simTime() - starttime;

		std::cout << "Runtime: " << runtime << std::endl;

		bitsSent += (int64) floor(runtime.dbl() * values.back()->datarate);
		std::cout << "Already " << bitsSent << " bits sent!" << std::endl;
		delay = this->currentDatarateDelay(packet->getBitLength() - bitsSent);

		// Add current time to delay
		controlInfo->setEarliestDelivery(simTime() + delay);

		EV << "Calculated earliest delivery for packet " << packet << " at " << controlInfo->getEarliestDelivery() << " (Delay: " << delay << ")" << std::endl;
	}
}

void TbusDatarateQueue::sendFrontOfQueue() {
	ASSERT2(queue.length() > 0, "Queue has to have length > 0!");
	cPacket* packet = queue.pop();

	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->removeControlInfo());
	ASSERT2(controlInfo, "Invalid control info on packet!");
	ASSERT2(controlInfo->getEarliestDelivery() <= simTime(), "Sending packet earlier than expected!");
	EV << "Dispatched packet " << packet << " after delay " << (simTime() - controlInfo->getQueueArrival()) << " (Entered Queue at " << controlInfo->getQueueArrival() << ")" << std::endl;
	delete controlInfo;

	// Check for drop
	if (dblrand() >= this->currentLossProbability()) {
		// No drop
		send(packet, outGate);
	} else {
		// Drop
		EV << "Packet " << packet << " dropped!" << std::endl;
		delete packet;
	}

	// Reset the bytes sent
	bitsSent = 0;
}

double TbusDatarateQueue::currentLossProbability() {
	ASSERT2(values.size() > 0, "Empty values array!");

	// Calculations according to Tobias Krauthoffs work
	simtime_t runtime = simTime() - values.front()->time;
	simtime_t endtime = simTime();
	simtime_t starttime;
	double loss = 0.0;

	reverseValueIterator it;
	for (it = values.rbegin(); it != values.rend(); ++it) {
		starttime = (*it)->time;
		loss += (*it)->droprate * (endtime.dbl() - starttime.dbl()) / runtime.dbl();
		endtime = starttime;
	}

	return loss;
}

simtime_t TbusDatarateQueue::currentDatarateDelay(int64_t bitLength) {
	ASSERT2(values.size() > 0, "Empty values array!");

	// Calculations according to Tobias Krauthoffs work
	simtime_t runtime = simTime() - values.front()->time;
	simtime_t endtime = simTime();
	simtime_t starttime;
	double datarate = 0.0;

	if (runtime == simtime_t()) {
		return ((double) bitLength) / values.front()->datarate;
	}

	reverseValueIterator it;
	for (it = values.rbegin(); it != values.rend(); ++it) {
		starttime = (*it)->time;
		datarate += (double) (*it)->datarate * (endtime.dbl() - starttime.dbl()) / runtime.dbl();
		endtime = starttime;
	}

	return ((double) bitLength) / datarate;
}

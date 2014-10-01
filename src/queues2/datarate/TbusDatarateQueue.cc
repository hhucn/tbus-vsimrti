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
	bytesSent(0) {
}

TbusDatarateQueue::~TbusDatarateQueue() {
}

void TbusDatarateQueue::calculateEarliestDeliveries() {
	packetIterator it(queue);
	for (; !it.end(); it++) {
		cPacket* packet = check_and_cast<cPacket*>(it());
		ASSERT2(packet, "Invalid packet in queue!");

		this->calculateEarliestDeliveryForPacket(packet);
	}
}

void TbusDatarateQueue::calculateEarliestDeliveryForPacket(cPacket* packet) {
	simtime_t delay;
	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->getControlInfo());
	ASSERT2(controlInfo, "Invalid control info on packet!");

	if (queue.front() == packet) {
		// Calculate earliest delivery with respect to already sent bytes

		// What time shall we use?
		simtime_t starttime = std::max(values.back()->time.dbl(), controlInfo->getQueueArrival().dbl());
		simtime_t runtime = starttime - simTime();

		bytesSent += (int64) floor(runtime.dbl() * values.back()->datarate);

		delay = this->currentDatarateDelay(packet->getByteLength() - bytesSent);
		controlInfo->setEarliestDelivery(simTime() + delay);
	} else {
		// Calculate earliest delivery with respect to packets in front of us
		// Get our delay
		delay = this->currentDatarateDelay(packet->getByteLength());

		// Add current delays of all packets until our packet
		packetIterator it(queue);
		for (; it() != packet; it++) {
			cPacket* other = check_and_cast<cPacket*>(it());
			ASSERT2(other, "invalid packet in queue!");
			TbusQueueControlInfo* otherControlInfo = check_and_cast<TbusQueueControlInfo*>(packet->getControlInfo());
			ASSERT2(otherControlInfo, "invalid control info on packet!");

			delay += otherControlInfo->getEarliestDelivery() - simTime();
		}

		// Add current time to delay
		controlInfo->setEarliestDelivery(simTime() + delay);
	}
}

void TbusDatarateQueue::sendFrontOfQueue() {
	ASSERT2(queue.length() > 0, "Queue has to have length > 0!");
	cPacket* packet = queue.pop();

	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->removeControlInfo());
	ASSERT2(controlInfo, "Invalid control info on packet!");
	ASSERT2(controlInfo->getEarliestDelivery() <= simTime(), "Sending packet earlier than expected!");
	delete controlInfo;

	// Check for drop
	if (dblrand() >= this->currentLossProbability()) {
		// No drop
		send(packet, outGate);
	} else {
		// Drop
		delete packet;
	}

	// Reset the bytes sent
	bytesSent = 0;

	values.clear();
	values.push_back(currentValue);
}

double TbusDatarateQueue::currentLossProbability() {
	ASSERT2(values.size() > 0, "Empty values array!");

	// Calculations according to Tobias Krauthoffs work
	simtime_t runtime = simTime() - values.front()->time;
	simtime_t endtime = simTime();
	simtime_t starttime;
	double loss = 0.0;

	valueIterator it;
	for (it = values.end(); it != values.begin(); --it) {
		starttime = (*it)->time;
		loss += (*it)->droprate * (endtime.dbl() - starttime.dbl()) / runtime.dbl();
		endtime = starttime;
	}

	return loss;
}

simtime_t TbusDatarateQueue::currentDatarateDelay(int64_t byteLength) {
	ASSERT2(values.size() > 0, "Empty values array!");

	// Calculations according to Tobias Krauthoffs work
	simtime_t runtime = simTime() - values.front()->time;
	simtime_t endtime = simTime();
	simtime_t starttime;
	double datarate = 0.0;

	if (runtime == simtime_t()) {
		return ((double) byteLength) / values.front()->datarate;
	}

	valueIterator it;
	for (it = values.end(); it != values.begin(); --it) {
		starttime = (*it)->time;
		datarate += (double) (*it)->datarate * (endtime.dbl() - starttime.dbl()) / runtime.dbl();
		endtime = starttime;
	}

	return ((double) byteLength) / datarate;
}

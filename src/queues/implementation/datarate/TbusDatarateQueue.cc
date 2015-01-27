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

#include "TbusDatarateQueue.h"
#include "TbusQueueControlInfo.h"

Define_Module(TbusDatarateQueue);

/**
 * Start with zero bits sent.
 */
TbusDatarateQueue::TbusDatarateQueue() : bitsSent(0) {}

/**
 * Only calculates the earliest delivery for head of queue (if there is one).
 */
void TbusDatarateQueue::calculateEarliestDeliveries() {
	// Only calculate for the front of queue
	if (!queue.empty()) {
		this->calculateEarliestDeliveryForPacket(queue.front());
	}
}

/**
 * Calculates earliest delivery for packet.
 * Earliest delivery depends on the current datarate and is calculated by using packet.size - bitsSent and maximum of packet arrival and previous value arrival.
 * @param packet Packet to calculate earliest delivery for
 */
void TbusDatarateQueue::calculateEarliestDeliveryForPacket(cPacket* packet) {
	// Only calculate for the first packet, the others have to wait
	if (queue.front() == packet) {
		// Calculate earliest delivery with respect to already sent bytes

		simtime_t delay;
		TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->getControlInfo());
		ASSERT2(controlInfo, "Invalid control info on packet!");

		if (values.size() > 1) {
			// What time shall we use?
			simtime_t starttime = SimTime(std::max(values[1]->time.inUnit(SIMTIME_NS), controlInfo->getHeadOfQueue().inUnit(SIMTIME_NS)), SIMTIME_NS);
			simtime_t runtime = simTime() - starttime;

			bitsSent += (int64) floor(runtime.dbl() * values[1]->datarate);
		}

		ASSERT2(bitsSent <= packet->getBitLength(), "Invalid amount of bits sent!");
		delay = this->currentDatarateDelay(packet->getBitLength() - bitsSent);

		// Add current time to delay
		controlInfo->setEarliestDelivery(simTime() + delay);

		EV << this->getName() << ": Calculated earliest delivery for packet " << packet << " at " << controlInfo->getEarliestDelivery() << " (Delay: " << delay << ")" << std::endl;
	}
}

/**
 * Dispatches head of queue packet.
 * Send the head of queue packet considering aggregated drop rates. Also clears the value deque for the next packet.
 */
void TbusDatarateQueue::sendFrontOfQueue() {
	cPacket* packet = getAndRemoveHeadOfQueue();

	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->getControlInfo());
	ASSERT2(controlInfo, "Invalid control info on packet!");
	ASSERT2(controlInfo->getEarliestDelivery() <= simTime(), "Sending packet earlier than expected!");
	EV << "Dispatched packet " << packet << " after delay " << (simTime() - controlInfo->getQueueArrival()) << " (Entered Queue at " << controlInfo->getQueueArrival() << ")" << std::endl;

	// Check for drop
	if (dblrand() >= this->currentLossProbability()) {
		// No drop
		send(packet, outGate);
	} else {
		// Drop
		EV << "Packet " << packet << " dropped!" << std::endl;
		delete packet;
	}

	// Remove the previous value
	TbusQueueDatarateValue* currentValue = new TbusQueueDatarateValue(*(values.front()));
	for (std::deque<TbusQueueDatarateValue*>::iterator it = values.begin(); it != values.end(); ++it) {
		delete *it;
	}
	values.clear();
	values.push_back(currentValue);

	// Reset the bytes sent
	bitsSent = 0;
}

/**
 * Aggregates the droprate over saved values and weights them according their time of presence
 * @return Aggregated weighted droprate
 */
double TbusDatarateQueue::currentLossProbability() {
	ASSERT2(!values.empty(), "Empty values array!");

	double loss;
	if (values.size() > 1) {
		// Calculations according to/adapted from Tobias Krauthoffs work
		simtime_t starttime;
		simtime_t endtime = simTime();
		simtime_t runtime = simTime() - values.back()->time;

		rValueIterator it;
		for (it = values.rbegin(); it != values.rend(); ++it) {
			starttime = (*it)->time;
			loss += (*it)->droprate * (endtime.dbl() - starttime.dbl()) / runtime.dbl();
			endtime = starttime;
		}
	} else {
		loss = values.front()->droprate;
	}

	return loss;
}

/**
 * Calculates the delay with current datarate.
 * Simply bitLength / currentDatarate.
 * @param bitLength Bits to calculate for
 * @return delay depending on bitLength and current datarate
 */
simtime_t TbusDatarateQueue::currentDatarateDelay(int64_t bitLength) {
	ASSERT2(!values.empty(), "Empty values array!");

	std::cout << "Calculated delay " << (((double) bitLength) / values.front()->datarate) << " for packet length " << bitLength << " and datarate " << values.front()->datarate << endl;

	return (((double) bitLength) / values.front()->datarate);
}

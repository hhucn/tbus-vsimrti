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
		calculateEarliestDeliveryForPacket(queue.front());
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

		// If more than two values are present, subtract already sent bytes from bytes to send
		if (values.size() > 1) {
			// What time shall we use?
			simtime_t starttime = SimTime(std::max(values[1]->time, controlInfo->getHeadOfQueue().inUnit(SIMTIME_NS)), SIMTIME_NS);
			simtime_t runtime = simTime() - starttime;

			bitsSent += (int64) floor(runtime.dbl() * values[1]->datarate);
		}

		ASSERT2(bitsSent <= packet->getBitLength(), "Invalid amount of bits sent!");
		delay = currentDatarateDelay(packet->getBitLength() - bitsSent);

		// Add current time to delay
		controlInfo->setEarliestDelivery(simTime() + delay);
		EV << this->getName() << ": Calculated earliest delivery for packet " << packet << " at " << controlInfo->getEarliestDelivery() << " (Delay: " << delay << ")" << std::endl;
		// Adapt our self message
		adaptSelfMessage();
	}
}

/**
 * Dispatches head of queue packet.
 * Send the head of queue packet considering aggregated drop rates. Also clears the value deque for the next packet.
 */
void TbusDatarateQueue::sendFrontOfQueue() {
	cPacket* packet = queue.pop();

	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->getControlInfo());
	ASSERT2(controlInfo->getEarliestDelivery() <= simTime(), "Sending packet earlier than expected!");
	EV << "Dispatching packet " << packet << " after delay " << (simTime() - controlInfo->getQueueArrival()) << " (Entered Queue at " << controlInfo->getQueueArrival() << ")" << std::endl;

	double currentLoss = currentLossProbability(controlInfo->getQueueArrival());

	// Check for drop
	if (uniform(0, 1) >= currentLoss) {
		// No drop
		send(packet, outGate);
		std::cout << simTime() << " - " << this->getName() << ": " << packet << " sent after " << (simTime() - controlInfo->getQueueArrival()) << ", added at " << controlInfo->getQueueArrival() << " current loss " << currentLoss << endl;
	} else {
		// Drop
		EV << "Packet " << packet << " dropped!" << std::endl;
		std::cout << simTime() << " - " << this->getName() << ": " << packet << " dropped after " << (simTime() - controlInfo->getQueueArrival()) << ", added at " << controlInfo->getQueueArrival() << " current loss " << currentLoss << endl;
		delete packet;
	}

	// Remove all but the first values
	clearAndDeleteValues(TBUS_CLEAR_ALL_EXCEPT_FRONT);

	// Reset the bytes sent
	bitsSent = 0;
}

/**
 * Aggregates the droprate over saved values and weights them according their time of presence
 * @param packetQueueArrivalTime Queue arrival time of the current packet to send
 * @return Aggregated weighted droprate
 */
double TbusDatarateQueue::currentLossProbability(simtime_t packetQueueArrivalTime) {
	ASSERT2(!values.empty(), "Empty values array!");

	double loss = 0.0;
	int64_t now = simTime().inUnit(SIMTIME_NS);
	int64_t queueArrival = packetQueueArrivalTime.inUnit(SIMTIME_NS);
	int64_t start;
	int64_t end = now;

	valueIterator endIterator = values.end();
	--endIterator;

	for (valueIterator it = values.begin(); it != endIterator; ++it) {
		start = (*it)->time;
		loss += (*it)->droprate * ((double) (end - start));
		end = start;
	}

	start = queueArrival;
	loss += values.back()->droprate * ((double) (end - start));

	// Normalize droprate
	loss /= (double) (now - queueArrival);

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

	return (((double) bitLength) / values.front()->datarate);
}

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

#include "TbusDelayQueue.h"

//Define_Module(TbusDelayQueue);

/**
 * Empty constructor.
 */
TbusDelayQueue::TbusDelayQueue(TbusQueueSelection selection) :
	TbusBaseQueue(selection) {}

/**
 * Calculates earliest delivery times for all packets enqueued.
 */
void TbusDelayQueue::calculateEarliestDeliveries() {
	simtime_t delay = currentDelay();

	// No iterators because OMNeTs iterators are weird
	for (int i = 0; i < queue.length(); i++) {
		calculateEarliestDeliveryForPacket(queue.get(i), delay);
	}
}

/**
 * Calculates earliest delivery time for packet with current delay
 * @see calculateEarliestDeliveryForPacket(cPacket*, simtime_t)
 * @see currentDelay()
 * @param packet Packet to calculate earliest delivery for
 */
void TbusDelayQueue::calculateEarliestDeliveryForPacket(cPacket* packet) {
	calculateEarliestDeliveryForPacket(packet, currentDelay());
}

/**
 * Calculates earliest delivery time for packet with delay.
 * @param packet Packet to calculate earliest delivery for
 * @param delay Delay to use
 */
void TbusDelayQueue::calculateEarliestDeliveryForPacket(cPacket* packet, simtime_t delay) {
	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->getControlInfo());

	// Calculations according to Tobias Krauthoffs work
	simtime_t delayGone = simTime() - controlInfo->getQueueArrival();
	simtime_t delayWait = delay - delayGone;

	EV << this->getName() << ": Delay: " << delay << ", delayGone: " << delayGone << ", delayWait: " << delayWait << endl;

	if (delayWait < SIMTIME_ZERO) {
		controlInfo->setEarliestDelivery(simTime());
	} else {
		controlInfo->setEarliestDelivery(simTime() + delayWait);
	}
	EV << this->getName() << ": Calculated earliest delivery for packet " << packet << " at " << controlInfo->getEarliestDelivery() << " (Delay: " << delay << ")" << std::endl;

	// Adapt our self message
	adaptSelfMessage();
}

/**
 * Current delay by iterating over saved values and weighting them according to their presence as head of value queue.
 * @return Aggregated weighted delay over all saved values
 */
simtime_t TbusDelayQueue::currentDelay() {
	ASSERT2(!values.empty(), "Empty values array!");
	// Calculations adapted from Tobias Krauthoffs work

	return SimTime(values.front()->delay, SIMTIME_NS);
}

//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusDelayQueue.h"

/**
 * Empty constructor.
 */
TbusDelayQueue::TbusDelayQueue(TbusQueueSelection selection) :
	TbusBaseQueue(selection) {}

/**
 * Calculates earliest delivery times for all packets enqueued.
 */
void TbusDelayQueue::calculateEarliestDeliveries() {
	// Only check first packet, other packets are sent back-to-back
	if (!queue.isEmpty()) {
		calculateEarliestDeliveryForPacket(queue.front());
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

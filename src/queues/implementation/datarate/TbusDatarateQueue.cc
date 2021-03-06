//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusDatarateQueue.h"
#include "TbusQueueControlInfo.h"

void TbusDatarateQueue::updateValue(TbusQueueDatarateValue* newValue) {
	if (newValue->datarate <= 0) {
		std::cout << "Received datarate " << newValue->datarate << " at " << simTime() << " node id " << getId() << std::endl;
	}

	TbusBaseQueue<TbusQueueDatarateValue>::updateValue(newValue);
}

/**
 * Start with zero bits sent.
 */
TbusDatarateQueue::TbusDatarateQueue(TbusQueueSelection selection) :
		TbusBaseQueue(selection),
		bitsSent(0) {}

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

			// TODO: Round statt floor?
			bitsSent += ((int64_t) round((runtime.inUnit(SIMTIME_NS) * values[1]->datarate) / 1000000000.0));
		}

		ASSERT2(bitsSent <= packet->getBitLength(), "Invalid amount of bits sent!");
		ASSERT2(bitsSent >= 0, "Negative amount of bits sent!");
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
#ifdef TBUS_DEBUG
		std::cout << simTime() << " - " << this->getName() << ": " << packet << " sent after " << (simTime() - controlInfo->getQueueArrival()) << ", added at " << controlInfo->getQueueArrival() << " current loss " << currentLoss << endl;
#endif /* TBUS_DEBUG */
	} else {
		// Drop
		EV << "Packet " << packet << " dropped!" << std::endl;
		std::cout << "Packet " << packet << " id " << packet->getId() << " dropped with currentLoss " << currentLoss << " and last loss probability " << values.back()->droprate << " and last datarate " << values.back()->datarate << " !" << std::endl;

		double loss = 0.0;
		int64_t now = simTime().inUnit(SIMTIME_NS);
		int64_t queueArrival = controlInfo->getQueueArrival().inUnit(SIMTIME_NS);
		int64_t start;
		int64_t end = now;

		valueIterator endIterator = values.end();
		--endIterator;

		for (valueIterator it = values.begin(); it != endIterator; ++it) {
			start = (*it)->time;
			loss += (*it)->droprate * ((double) (end - start));

			std::cout << "(" << (*it)->droprate << ", " << ((double) end - start) << ") ";
			end = start;
		}

		start = queueArrival;
		loss += values.back()->droprate * ((double) (end - start));
		std::cout << "(" << values.back()->droprate << ", " << ((double) end - start) << ") " << std::endl;

		std::cout << loss << " normalize by " << ((double)  now - queueArrival);
		// Normalize droprate
		loss /= (double) (now - queueArrival);

		std::cout << " calculated " << loss << std::endl;

#ifdef TBUS_DEBUG
		std::cout << simTime() << " - " << this->getName() << ": " << packet << " dropped after " << (simTime() - controlInfo->getQueueArrival()) << ", added at " << controlInfo->getQueueArrival() << " current loss " << currentLoss << endl;
#endif /* TBUS_DEBUG */
		delete packet;
	}

	// Remove all but the first values
	clearAndDeleteValues(TBUS_CLEAR_ALL_EXCEPT_FRONT);

	// Reset the bits sent
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

/**
 * Set ACTIVE status as CELL_ACTIVE and any other status as status.
 * @param status Queue status
 */
void TbusDatarateQueue::setQueueStatus(TbusQueueStatus status) {
	if (status == ACTIVE) {
		TbusBaseQueue::setQueueStatus(CELL_ACTIVE);
	} else {
		TbusBaseQueue::setQueueStatus(status);
	}
}

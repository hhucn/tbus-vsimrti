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

Define_Module(TbusDelayQueue);

TbusDelayQueue::TbusDelayQueue() : TbusBaseQueue<TbusQueueDelayValue>() {
}

void TbusDelayQueue::calculateEarliestDeliveries() {
	simtime_t delay = this->currentDelay();

	// No iterators because OMNeTs iterators are weird
	for (int i = 0; i < queue.length(); i++) {
		this->calculateEarliestDeliveryForPacket(queue.get(i), delay);
	}
}

void TbusDelayQueue::calculateEarliestDeliveryForPacket(cPacket* packet) {
	this->calculateEarliestDeliveryForPacket(packet, this->currentDelay());
}

void TbusDelayQueue::calculateEarliestDeliveryForPacket(cPacket* packet, simtime_t delay) {
	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(packet->getControlInfo());
	ASSERT2(controlInfo, "Invalid control info on packet!");

	// Calculations according to Tobias Krauthoffs work
	simtime_t delayGone = simTime() - controlInfo->getQueueArrival();
	simtime_t delayWait = delay - delayGone;

	if (delayWait < simtime_t()) {
		controlInfo->setEarliestDelivery(simTime());
	} else {
		controlInfo->setEarliestDelivery(simTime() + delayWait);
	}
}

simtime_t TbusDelayQueue::currentDelay() {
	ASSERT2(!values.empty(), "Empty values array!");
	// Calculations according to/adapted from Tobias Krauthoffs work

	simtime_t delay;
	if (values.size() > 1) {
		simtime_t starttime;
		simtime_t endtime = simTime();
		simtime_t runtime = simTime() - values.back()->time;

		rValueIterator it;
		for (it = values.rbegin(); it != values.rend(); ++it) {
			starttime = (*it)->time;
			delay += (*it)->delay.dbl() * (endtime - starttime) / runtime;
			endtime = starttime;
		}
	} else {
		delay = values.front()->delay;
	}

	return delay;
}

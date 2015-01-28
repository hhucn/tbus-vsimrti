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

#include <control_info/TbusQueueControlInfo.h>

/**
 * Empty constructor
 */
TbusQueueControlInfo::TbusQueueControlInfo() {}

/**
 * Empty destructor
 */
TbusQueueControlInfo::~TbusQueueControlInfo() {}

/**
 * Time for earliest delivery from qeue
 * @return earliest delivery time
 */
simtime_t TbusQueueControlInfo::getEarliestDelivery() {
	return earliestDelivery;
}

/**
 * Arrival time at queue
 * @return arrival time
 */
simtime_t TbusQueueControlInfo::getQueueArrival() {
	return queueArrival;
}

/**
 * Time at which the packet became head of queue
 * @return head of queue time
 */
simtime_t TbusQueueControlInfo::getHeadOfQueue() {
	return headOfQueue;
}

/**
 * Setter for queue arrival time
 * @param time Time to be set for queue arrival
 */
void TbusQueueControlInfo::setQueueArrival(simtime_t time) {
	queueArrival = time;
}

/**
 * Setter for earliest delivery time
 * @param time Time to set for earliest delivery
 */
void TbusQueueControlInfo::setEarliestDelivery(simtime_t time) {
	earliestDelivery = time;
}

/**
 * Setter for head of queue time
 * @param time Time to set for head of queue
 */
void TbusQueueControlInfo::setHeadOfQueue(simtime_t time) {
	headOfQueue = time;
}

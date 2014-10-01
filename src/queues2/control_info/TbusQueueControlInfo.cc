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
 * Saves simulation time on creation in queueArrival
 */
TbusQueueControlInfo::TbusQueueControlInfo() : queueArrival(simTime()) {
}

TbusQueueControlInfo::~TbusQueueControlInfo() {
	// TODO Auto-generated destructor stub
}

/**
 * Time for earliest delivery from qeue
 * @return earliest delivery time
 */
const simtime_t& TbusQueueControlInfo::getEarliestDelivery() const {
	return earliestDelivery;
}

/**
 * Arrival time at queue (=> Object creation time)
 * @return arrival time
 */
const simtime_t& TbusQueueControlInfo::getQueueArrival() const {
	return queueArrival;
}

void TbusQueueControlInfo::setEarliestDelivery(simtime_t time) {
	earliestDelivery = time;
}

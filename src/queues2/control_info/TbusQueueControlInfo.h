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

#ifndef TBUSQUEUECONTROLINFO_H_
#define TBUSQUEUECONTROLINFO_H_

#include <cobject.h>
#include "omnetpp.h"

/**
 * Packet control info for arrival and earliest delivery time
 */
class TbusQueueControlInfo : public cObject {
	private:
		simtime_t queueArrival;
		simtime_t earliestDelivery;
	public:
		TbusQueueControlInfo();
		virtual ~TbusQueueControlInfo();

		simtime_t getQueueArrival();
		simtime_t getEarliestDelivery();

		void setEarliestDelivery(simtime_t time);
};

#endif /* TBUSQUEUECONTROLINFO_H_ */

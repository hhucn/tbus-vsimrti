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

#ifndef TBUSDATARATEQUEUE_H_
#define TBUSDATARATEQUEUE_H_

#include <base/TbusBaseQueue.h>
#include "TbusQueueDatarateValue.h"

class TbusDatarateQueue : public TbusBaseQueue<TbusQueueDatarateValue> {
	public:
		TbusDatarateQueue();
		virtual ~TbusDatarateQueue();

	protected:
		int64_t bytesSent;

		virtual void calculateEarliestDeliveries();
		virtual void calculateEarliestDeliveryForPacket(cPacket* packet);

		virtual void sendFrontOfQueue();

		double currentLossProbability();
		simtime_t currentDatarateDelay(int64_t byteLength);
};

#endif /* TBUSDATARATEQUEUE_H_ */

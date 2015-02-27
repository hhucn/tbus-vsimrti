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

#include "TbusBaseQueue.h"
#include "TbusQueueDatarateValue.h"

/**
 * Queue acting as air channel.
 * This implementation acts as a base for air channel delay and dropping-
 */
class TbusDatarateQueue : public TbusBaseQueue<TbusQueueDatarateValue> {
	public:
		TbusDatarateQueue(TbusQueueSelection selection);

		bool isCellActive() const;

	protected:
		double bitsSent; ///< Bits already sent for head of queue (Needed for multiple data- and droprate value per packet)

		virtual void calculateEarliestDeliveries();
		virtual void calculateEarliestDeliveryForPacket(cPacket* packet);

		virtual void sendFrontOfQueue();

		inline double currentLossProbability(simtime_t packetQueueArrivalTime);
		inline simtime_t currentDatarateDelay(int64_t byteLength);

		virtual void setQueueStatus(TbusQueueStatus status);
};

#endif /* TBUSDATARATEQUEUE_H_ */

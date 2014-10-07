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

#ifndef TBUSCDSQ_H_
#define TBUSCDSQ_H_

#define TBUS_DELAY_QUEUE_START_SAVE_VALUES "tbus.delay.queue.start.save.values"
#define TBUS_DELAY_QUEUE_STOP_SAVE_VALUES "tbus.delay.queue.stop.save.values"
#define TBUS_DELAY_QUEUE_RESET_VALUES "tbus.delay.queue.reset.values"

#include "TbusDelayQueue.h"
#include "TbusQueueDelayValue.h"

class TbusCDSQ : public TbusDelayQueue {
	private:
		bool saveValues;

	public:
		TbusCDSQ();

		virtual void updateValue(TbusQueueDelayValue* newValue);
		virtual void handleMessage(cMessage* msg);

	protected:
		virtual void handleSelfMessage(cMessage* msg);
		virtual void calculateEarliestDeliveries();
};

#endif /* TBUSCDSQ_H_ */

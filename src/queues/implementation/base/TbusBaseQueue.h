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

#ifndef TBUSBASEQUEUE_H_
#define TBUSBASEQUEUE_H_

#include "csimplemodule.h"
#include "cmessage.h"
#include "TbusPacketQueue.h"
#include "TbusQueueControlInfo.h"
#include "TbusQueueValue.h"
#include "TbusQueueSelection.h"
#include "TbusQueueStatus.h"
#include "TbusQueueControlCallback.h"

#include <deque>

/**
 * Enum parameter for clearing value list
 */
enum TbusClearMethod {
	TBUS_CLEAR_ALL,            ///< TBUS_CLEAR_ALL
	TBUS_CLEAR_ALL_EXCEPT_FRONT///< TBUS_CLEAR_ALL_EXCEPT_FRONT
};

/**
 * Base class for all TBUS queues.
 * Provides a generic interface.
 */
template <class T> class TbusBaseQueue : public cSimpleModule {
	public:
		TbusBaseQueue(TbusQueueSelection selection);
		virtual ~TbusBaseQueue();

		virtual void initialize();
		virtual void handleMessage(cMessage* msg);
		virtual void finish();

		virtual void updateValue(T* newValue);

		void setQueueControlCallback(TbusQueueControlCallback* qcCallback);

		virtual TbusQueueStatus getQueueStatus() const;

	protected:
		TbusPacketQueue queue; ///< The packet queue
		cMessage selfMessage; ///< Self message for timed events
		TbusQueueControlCallback* callback; ///< Callback object to Queue Control
		TbusQueueSelection queueSelection; ///< Queue selection
		TbusQueueStatus queueStatus; ///< Queue status

		int inGate; ///< Input gate id
		int outGate; ///< output gate id

		std::deque<T*> values; ///< Value deque
		typedef typename std::deque<T*>::iterator valueIterator; ///< Value deque iterator

		void setQueueLength(int64_t queueLength);

		inline void clearAndDeleteValues(const TbusClearMethod method = TBUS_CLEAR_ALL);

		virtual void handleSelfMessage(cMessage* msg);
		virtual bool addPacketToQueue(cPacket* packet);

		virtual void setQueueStatus(TbusQueueStatus status);

		void adaptSelfMessage();
		virtual void calculateEarliestDeliveries() = 0;
		virtual void calculateEarliestDeliveryForPacket(cPacket* packet) = 0;

		virtual void sendFrontOfQueue();
		cPacket* getAndRemoveHeadOfQueue();
};

#endif /* TBUSBASEQUEUE_H_ */

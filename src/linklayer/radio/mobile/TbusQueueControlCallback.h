/*
 * TbusCellShareCallback.h
 *
 *  Created on: 23.02.2015
 *      Author: bialon
 */

#ifndef TBUSQUEUECONTROLCALLBACK_H_
#define TBUSQUEUECONTROLCALLBACK_H_

#include "TbusQueueSelection.h"
#include "TbusQueueStatus.h"

class TbusQueueControlCallback {
	public:
		/**
		 * Update queue values from database for a given selection.
		 * @param selection Queue selection
		 */
		virtual void updateQueueValuesFromDatabase(TbusQueueSelection selection) = 0;

		/**
		 * Adapt all active queue values with CellShare calculations.
		 * @param selection Queue selection
		 */
		virtual void adaptQueueValues(TbusQueueSelection selection) = 0;

		/**
		 * Make queue control aware of a queue status change.
		 */
		virtual void queueStatusChanged() = 0;

		/**
		 * Get the highest status of all queues.
		 * @return Highest queue status
		 */
		virtual TbusQueueStatus getQueueStatus() const = 0;
};

#endif /* TBUSQUEUECONTROLCALLBACK_H_ */

/*
 * TbusCellShareCallback.h
 *
 *  Created on: 23.02.2015
 *      Author: bialon
 */

#ifndef TBUSCELLSHARECALLBACK_H_
#define TBUSCELLSHARECALLBACK_H_

enum TbusQueueSelection {
	CDRQ = 1,
	CRRQ = 2,
	CDSQ = 4,
	CRSQ = 8,
	ALL  = 15
};

class TbusCellShareCallback {
	public:
		virtual void triggerQueueValueUpdate(TbusQueueSelection) = 0;
		virtual bool isActive() const = 0;
};

#endif /* TBUSCELLSHARECALLBACK_H_ */

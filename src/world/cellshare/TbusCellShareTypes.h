/*
 * TbusCellShareTypes.h
 *
 *  Created on: 20.02.2015
 *      Author: bialon
 */

#ifndef TBUSCELLSHARETYPES_H_
#define TBUSCELLSHARETYPES_H_

#include "TbusCellShareCallback.h"

typedef int64_t cellid_t;
#define TBUS_INVALID_CELLID ((cellid_t) -1)

class cModule;

/**
 * A TbusHost contains a host reference and queueControl reference.
 */
struct TbusHost {
	cModule* host; ///< Host reference
	TbusCellShareCallback* callback; ///< Callback object reference
	bool isActive;
};

#endif /* TBUSCELLSHARETYPES_H_ */

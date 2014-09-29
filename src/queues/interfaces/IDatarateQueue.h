/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 28.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */
/**
 * Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#ifndef IDATARATEQUEUE_H_
#define IDATARATEQUEUE_H_

namespace projekt {

#include <omnetpp.h>
#include "Datarate.h"

class IDatarateQueue {

	public:
		virtual void datarateChanged(Datarate* newDatarate) = 0;

	protected:
		virtual void dispatch(bool retBool[]) = 0;
};
};

#endif /* IDATARATEQUEUE_H_ */

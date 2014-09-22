/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 28.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef IBBDELAYQUEUE_H_
#define IBBDELAYQUEUE_H_
namespace projekt {

#include <omnetpp.h>
#include "Bbdelay.h"

class IBbdelayQueue {

	public:
		virtual void bbDelayChanged(Bbdelay* newBbdelay, simtime_t currentSimtime) = 0;

	protected:
		virtual void dispatch(simtime_t currentSimtime) = 0;
}
;
}
;

#endif /* IBBDELAYQUEUE_H_ */

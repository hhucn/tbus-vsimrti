/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 28.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef IDATARATEQUEUE_H_
#define IDATARATEQUEUE_H_
namespace projekt {

#include <omnetpp.h>
#include "Datarate.h"

class IDatarateQueue{

	public:
		virtual void datarateChanged(Datarate* newDatarate, simtime_t currentSimtime) = 0;

	protected:
		virtual void dispatch(simtime_t currentSimtime, bool retBool[]) = 0;
}
;
}
;

#endif /* IDATARATEQUEUE_H_ */

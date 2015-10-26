//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusCRRQ.h"

Define_Module(TbusCRRQ);

/**
 * Empty constructor.
 */
TbusCRRQ::TbusCRRQ() : TbusDatarateQueue(CRRQ) {}

void TbusCRRQ::initialize() {
	setQueueLength(par("queueLength").longValue());

	TbusDatarateQueue::initialize();
}

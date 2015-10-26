#include "TbusMobileNode.h"
#include "ModuleAccess.h"

Define_Module(TbusMobileNode);

TbusMobileNode::TbusMobileNode() : VSimRTIExtendedMobilityNode() {}

TbusMobileNode::~TbusMobileNode() {}

/**
 * Module initialisation.
 * Get QueueControl and WorldView references, register QueueControl at WorldView.
 * @param stage initialisation stage
 */
void TbusMobileNode::initialize(int stage) {
	if (stage == 0) {
		qc = ModuleAccess<TbusQueueControl>("queueControl").get();
		wv = TbusWorldView::getInstance();

		wv->registerQueueControl(qc);
	}
}

/**
 * Called upon node removal.
 * Unregister from WorldView.
 */
void TbusMobileNode::finish() {
	wv->unregisterQueueControl(qc);
}

/**
 * Called upon mobility update completion.
 */
void TbusMobileNode::extendedMobilityUpdated() {
	// TODO: Use real values
	wv->nodeMoved(qc, getRoadId(), getLanePos());
}

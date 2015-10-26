#include "TbusWorldView.h"

TbusWorldView::TbusWorldView() : currentUpdated(0) {}

TbusWorldView::~TbusWorldView() {}

/**
 * Get a singleton instance.
 * @return Singleton instance
 */
TbusWorldView* TbusWorldView::getInstance() {
	static TbusWorldView* instance = new TbusWorldView();

	return instance;
}

/**
 * Register a queue control to world view.
 * @param qc Queue control
 */
void TbusWorldView::registerQueueControl(TbusQueueControl* qc) {
	queueControls.insert(qc);
}

/**
 * Unregister a queue control from world view.
 * @param qc Queue control
 */
void TbusWorldView::unregisterQueueControl(TbusQueueControl* qc) {
	queueControls.erase(qc);
}

/**
 * Make QueueControl qc aware of a node movement by setting a new (roadId, lanePos) position.
 * @param qc Queue control
 * @param roadId Road id
 * @param lanePos Lane position
 */
void TbusWorldView::nodeMoved(TbusQueueControl* qc, const char* const roadId, const float lanePos) {
	qc->setRoadId(roadId);
	qc->setLanePos(lanePos);

	currentUpdated++;

	if (queueControls.size() == currentUpdated) {
		currentUpdated = 0;

		performUpdateRound();
	}
}

/**
 * Performs one update round with two inner rounds:
 * - Update cell id and queue values
 * - Set adapted queue values
 */
void TbusWorldView::performUpdateRound() {
	QueueControlSet::iterator it;

	// Begin cell id and queue value update round
	for (it = queueControls.begin(); it != queueControls.end(); ++it) {
		(*it)->updateCellIdFromDatabase();
		(*it)->updateQueueValuesFromDatabase(ALL);
	}

	// Begin adapted value assignment round
	for (it = queueControls.begin(); it != queueControls.end(); ++it) {
		(*it)->adaptQueueValues(ALL);
	}
}

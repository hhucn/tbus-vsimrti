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

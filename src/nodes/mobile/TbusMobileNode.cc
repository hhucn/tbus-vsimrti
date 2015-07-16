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

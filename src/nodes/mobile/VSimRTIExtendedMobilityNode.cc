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

#include "VSimRTIExtendedMobilityNode.h"
#include "omnetpp.h"

VSimRTIExtendedMobilityNode::VSimRTIExtendedMobilityNode():
	currentRoadId(NULL) {
}

VSimRTIExtendedMobilityNode::~VSimRTIExtendedMobilityNode() {
	if (currentRoadId != NULL) delete[] currentRoadId;
}

void VSimRTIExtendedMobilityNode::setRoadId(const char* roadId) {
	if (currentRoadId != NULL) delete[] currentRoadId;
	currentRoadId = roadId;
}

void VSimRTIExtendedMobilityNode::setLanePos(float lanePos) {
	currentLanePos = lanePos;
}

const char* const VSimRTIExtendedMobilityNode::getRoadId() {
	return currentRoadId;
}

const float VSimRTIExtendedMobilityNode::getLanePos() {
	return currentLanePos;
}

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

#ifndef VSIMRTIEXTENDEDMOBILITYNODE_H_
#define VSIMRTIEXTENDEDMOBILITYNODE_H_

class VSimRTIExtendedMobilityNode {
	private:
		const char* currentRoadId;
		float currentLanePos;

	public:
		VSimRTIExtendedMobilityNode();
		virtual ~VSimRTIExtendedMobilityNode();

		void setRoadId(const char* roadId);
		void setLanePos(float lanePos);

		const char* const getRoadId();
		const float getLanePos();

		virtual void extendedMobilityUpdated() = 0;
};

#endif /* VSIMRTIEXTENDEDMOBILITYNODE_H_ */

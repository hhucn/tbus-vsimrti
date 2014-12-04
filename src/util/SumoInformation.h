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

#ifndef SUMOINFORMATION_H_
#define SUMOINFORMATION_H_

#include <pthread.h>
#include <utility>
#include <map>
#include <string>
#include <cstddef>
#include <omnetpp.h>

#include "LanePosition.h"

class SumoInformation {
	private:
		pthread_t clientThread;
		pthread_mutex_t writeMutex;
		pthread_mutex_t startMutex;

		std::map<std::string, lanePos_t> positions;

		bool running;

		static void* run(void* instance);

	public:
		SumoInformation();
		virtual ~SumoInformation();

		void initialize(int stage);
		int numStages() {return 5;};
		void finish();

		lanePos_t getLanePosition(std::string& vehicleId);
};

#endif /* SUMOINFORMATION_H_ */

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

#include "SumoInformation.h"
#include "SumoInformationChannel.h"

SumoInformation::SumoInformation() {
	pthread_mutex_init(&writeMutex, NULL);
	pthread_mutex_init(&startMutex, NULL);
}

SumoInformation::~SumoInformation() {}

void* SumoInformation::run(void* instance) {
	SumoInformation* si = static_cast<SumoInformation*>(instance);
	SumoInformationChannel sichannel;

	pthread_mutex_lock(&si->startMutex);
	if (!sichannel.connectToServer("localhost", "3330")) {
		cRuntimeError("Cannot connect to TBUS VSimRTI Application for SUMO information!");
		return NULL;
	}
	pthread_mutex_unlock(&si->startMutex);

	while (si->running) {
		lanePos_t position = sichannel.readLanePosition();

		pthread_mutex_lock(&si->writeMutex);
		std::pair<std::map<std::string, lanePos_t>::iterator, bool> result = si->positions.insert(std::pair<std::string, lanePos_t>(position.vehicleId, position));

		if (result.second == false) {
			result.first->second = position;
		}
		pthread_mutex_unlock(&si->writeMutex);
	}

	return NULL;
}

void SumoInformation::initialize(int stage) {
	if (stage == 4) {
		running = true;

		if (pthread_create(&clientThread, NULL, &run, this) != 0) {
			cRuntimeError("Cannot create SUMO Information Thread!");
		}

		// Wait for client to connect
		pthread_mutex_lock(&startMutex);
		pthread_mutex_unlock(&startMutex);
	}
}

void SumoInformation::finish() {
	pthread_cancel(clientThread);
	pthread_join(clientThread, NULL);
	pthread_mutex_destroy(&writeMutex);
	pthread_mutex_destroy(&startMutex);
}

lanePos_t SumoInformation::getLanePosition(std::string& vehicleId) {
	lanePos_t pos;

	pthread_mutex_lock(&writeMutex);
	pos = positions.find(vehicleId)->second;
	pthread_mutex_unlock(&writeMutex);

	return pos;
}

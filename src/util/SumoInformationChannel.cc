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

#include "SumoInformationChannel.h"
#include <unistd.h>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SocketErrno errno

#define INVALID_SOCKET -1

#define SUMO_INFORMATION_DEBUG

SumoInformationChannel::SumoInformationChannel() {
	sock = INVALID_SOCKET;
}

SumoInformationChannel::~SumoInformationChannel() {
	if (sock >= 0) {
		close(sock);
		sock = -1;

#ifdef SUMO_INFORMATION_DEBUG
	std::cout << "Successfully closed socket" << std::endl;
#endif
	}
}

bool SumoInformationChannel::connectToServer(std::string host, std::string port) {
	struct addrinfo* server;
	struct addrinfo hints;

	memset(&hints, 0x00, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int result = getaddrinfo(host.c_str(), port.c_str(), &hints, &server);
	if (result != 0) {
		std::cerr << __FILE__ << ":" << __LINE__ << " getaddrinfo error: " << gai_strerror(result) << std::endl;
		return false;
	}

	sock = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
	if (sock < 0) {
		std::cerr << __FILE__ << ":" << __LINE__ << " Cannot create socket!" << std::endl;
		return false;
	}

	if (connect(sock, server->ai_addr, server->ai_addrlen) < 0) {
		std::cerr << __FILE__ << ":" << __LINE__ << " Cannot connect to " << host << " on port " << port << std::endl;
		return false;
	}

#ifdef SUMO_INFORMATION_DEBUG
	std::cout << "Successfully connected to " << host << " on port " << port << std::endl;
#endif

	freeaddrinfo(server);

	return true;
}

uint32_t SumoInformationChannel::readInt() {
	union {
		char c[4];
		uint32_t i;
	} buffer;

	for (uint32_t i = 0; i < 4; ++i) {
		if (recv(sock, &buffer.c[i], 1, 0) == -1) {
			std::cerr << __FILE__ << ":" << __LINE__ << " Error while reading int from socket!" << std::endl;
			return 0;
		}
	}

#ifdef SUMO_INFORMATION_DEBUG
	std::cout << "Read int: " << ntohl(buffer.i) << std::endl;
#endif

	return ntohl(buffer.i);
}

std::string SumoInformationChannel::readString() {
	uint32_t stringSize = readInt();
	std::vector<char> buffer(stringSize);

	if (recv(sock, buffer.data(), stringSize, 0) == -1) {
		std::cerr << __FILE__ << ":" << __LINE__ << " Error while reading string from socket!" << std::endl;
		return "";
	}

#ifdef SUMO_INFORMATION_DEBUG
	std::cout << "Read string: " << std::string().append(buffer.begin(), buffer.end()) << std::endl;
#endif

	return std::string().append(buffer.begin(), buffer.end());
}

double_t SumoInformationChannel::readDouble() {
	union {
		char c[8];
		uint32_t i[2];
		double_t d;
	} buffer;

	for (uint32_t i = 0; i < 8; ++i) {
		if (recv(sock, &buffer.c[i], 1, 0) == -1) {
			std::cerr << __FILE__ << ":" << __LINE__ << " Error while reading double from socket!" << std::endl;
			return 0.0;
		}
	}

	uint32_t temp = buffer.i[0];
	buffer.i[0] = ntohl(buffer.i[1]);
	buffer.i[1] = ntohl(temp);

#ifdef SUMO_INFORMATION_DEBUG
	std::cout << "Read double: " << buffer.d << std::endl;
#endif

	return buffer.d;
}

lanePos_t SumoInformationChannel::readLanePosition() {
	lanePos_t pos;

	pos.vehicleId = readString();
	pos.laneId    = readString();
	pos.distance  = readDouble();

#ifdef SUMO_INFORMATION_DEBUG
	std::cout << "Read Lane Position: {vehicleId: " << pos.vehicleId << ", laneId: " << pos.laneId << ", distance: " << pos.distance << "}" << std::endl;
#endif

	return pos;
}

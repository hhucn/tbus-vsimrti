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

#ifndef SUMOINFORMATIONCHANNEL_H_
#define SUMOINFORMATIONCHANNEL_H_

#ifdef WINDOWS
#error "This Class is not intended to be used with Microsoft Windows!"
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define SocketErrno errno

#define SOCKET int
#define INVALID_SOCKET -1

#include <string.h>
#include <float.h>
#include "LanePosition.h"

class SumoInformationChannel {
	private:
		SOCKET sock;

		uint32_t readInt();
		std::string readString();
		double_t readDouble();

	public:
		SumoInformationChannel();
		virtual ~SumoInformationChannel();

		bool connectToServer(std::string host, int port);
		lanePos_t readLanePosition();
};

#endif /* SUMOINFORMATIONCHANNEL_H_ */

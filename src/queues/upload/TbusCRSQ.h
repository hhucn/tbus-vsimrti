//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
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

#ifndef TBUSCRSQ_H_
#define TBUSCRSQ_H_

#include "TbusDatarateQueue.h"

/**
 * Client dataRate Send Queue.
 * Behaves like a #TbusDatarateQueue, but informs the CDSQ of when to start/stop saving values or resetting saved values.
 */
class TbusCRSQ : public TbusDatarateQueue {
	public:
		TbusCRSQ();
		virtual void initialize();

	protected:
		virtual bool addPacketToQueue(cPacket* packet);
		virtual void sendFrontOfQueue();
};

#endif /* TBUSCRSQ_H_ */

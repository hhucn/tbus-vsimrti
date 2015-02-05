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

#ifndef TBUSQUEUECONTROL_H_
#define TBUSQUEUECONTROL_H_

#include "csimplemodule.h"

#include "TbusCDRQ.h"
#include "TbusCRRQ.h"
#include "TbusCRSQ.h"
#include "TbusCDSQ.h"
#include "Coord.h"
#include "DatabaseHandler.h"
#include "TbusCoordinateConverter.h"

#define TBUS_QUEUE_TESTING

/**
 * Queue control class.
 * Distributes network characteristics along the related queues.
 */
class TbusQueueControl : public cSimpleModule {
	private:
		TbusCDRQ* cdrq; ///< Client delay receive queue reference
		TbusCRRQ* crrq; ///< Client datarate receive queue reference
		TbusCRSQ* crsq; ///< Client datarate send queue reference
		TbusCDSQ* cdsq; ///< Client delay send queue reference

		DatabaseHandler* dbHandler; ///< Database handler reference
		TbusCoordinateConverter* converter; ///< Coordinate converter reference

	public:
		TbusQueueControl();

		void updateQueues(const Coord& newCoords);
		void updateQueues(const char* const roadId, const float lanePos);

		void initialize();

#ifdef TBUS_QUEUE_TESTING
		void handleMessage(cMessage* msg);
#endif
};

#endif /* TBUSQUEUECONTROL_H_ */

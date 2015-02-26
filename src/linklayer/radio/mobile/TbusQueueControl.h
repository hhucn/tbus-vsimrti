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
#include "TbusDatabaseHandler.h"
#include "TbusCellShareTypes.h"
#include "TbusQueueControlCallback.h"
#include "TbusCoordinateConverter.h"

#define TBUS_QUEUE_TESTING

class TbusCellShare;

/**
 * Queue control class.
 * Distributes network characteristics along the related queues.
 */
class TbusQueueControl : public cSimpleModule, public TbusQueueControlCallback {
	private:
		TbusCDRQ* cdrq; ///< Client delay receive queue reference
		TbusCRRQ* crrq; ///< Client datarate receive queue reference
		TbusCRSQ* crsq; ///< Client datarate send queue reference
		TbusCDSQ* cdsq; ///< Client delay send queue reference

		TbusQueueDelayValue* 	cdrqValue;	///< Current CDRQ value from database
		TbusQueueDatarateValue*	crrqValue;	///< Current CRRQ value from database
		TbusQueueDatarateValue*	crsqValue;	///< Current CRSQ value from database
		TbusQueueDelayValue*	cdsqValue;	///< Current CDSQ value from database

		TbusDatabaseHandler* dbHandler; ///< Database handler reference
		TbusCellShare* cellShare; ///< Cell share calculation model

		cellid_t currentCellId; ///< Current cell id
		char* currentRoadId; ///< Current road id
		float currentLanePos; ///< Current lane position

		TbusHost tbusHost;

	public:
		TbusQueueControl();

		void setRoadId(const char* roadId);
		void setLanePos(float lanePos);

		void updateCellIdFromDatabase();
		virtual void updateQueueValuesFromDatabase(TbusQueueSelection selection);
		virtual void adaptQueueValues(TbusQueueSelection selection);

		virtual void queueStatusChanged();
		virtual TbusQueueStatus getQueueStatus() const;

		virtual void initialize();
		virtual void finish();

#ifdef TBUS_QUEUE_TESTING
		void handleMessage(cMessage* msg);
#endif
};

#endif /* TBUSQUEUECONTROL_H_ */

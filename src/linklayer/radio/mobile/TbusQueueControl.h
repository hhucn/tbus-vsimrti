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
#include "TbusDatabaseHandler.h"
#include "TbusCellShareTypes.h"
#include "TbusQueueControlCallback.h"

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

		uint64_t cdrq_defaultDelay;
		uint64_t cdsq_defaultDelay;
		uint64_t crrq_defaultDatarate;
		double crrq_defaultLossProbability;
		uint64_t crsq_defaultDatarate;
		double crsq_defaultLossProbability;

		TbusDatabaseHandler* dbHandler; ///< Database handler reference
		TbusCellShare* cellShare; ///< Cell share calculation model

		cellid_t currentCellId; ///< Current cell id
		char* currentRoadId; ///< Current road id
		float currentLanePos; ///< Current lane position

		bool roadIdValid;

		TbusHost tbusHost;

	public:
		TbusQueueControl();
		~TbusQueueControl();

		void setRoadId(const char* roadId);
		void setLanePos(float lanePos);

		void updateCellIdFromDatabase();
		virtual void updateQueueValuesFromDatabase(TbusQueueSelection selection);
		virtual void adaptQueueValues(TbusQueueSelection selection);

		virtual void queueStatusChanged(TbusQueueSelection status);
		virtual TbusQueueStatus getQueueStatus() const;

		virtual void initialize();
		virtual void finish();
};

#endif /* TBUSQUEUECONTROL_H_ */

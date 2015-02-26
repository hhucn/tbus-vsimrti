/*
 * TbusCellShare.h
 *
 *  Created on: 17.02.2015
 *      Author: bialon
 */

#ifndef TBUSCELLSHARE_H_
#define TBUSCELLSHARE_H_

#include "omnetpp.h"
#include "TbusQueueDatarateValue.h"
#include "TbusQueueDelayValue.h"
#include "TbusCellShareTypes.h"

#include <map>
#include <set>

/**
 * Representation of a TBUS cell share module.
 */
class TbusCellShare {
	protected:
		/**
		 * Protected constructor for inheriting.
		 */
		TbusCellShare() {};

		typedef std::set<TbusHost*> HostSet; ///< Set of hosts in cell

		/**
		 * Returns the number of active hosts in the given cell.
		 * @param cellId Cell id of cell
		 * @return Number of active hosts
		 */
		uint64_t getActiveHostsInCell(cellid_t cellId) {
			HostSet::iterator it;
			uint64_t numActive = 0;

			for (it = cellToHosts[cellId].begin(); it != cellToHosts[cellId].end(); ++it) {
				// Only count status higher than ACTIVE, we need CELL_ACTIVE
				if ((*it)->callback->getQueueStatus() > ACTIVE) {
					numActive++;
				}
			}

			return numActive;
		}

		/**
		 * Get a set of all hosts currently connected to a cell with cell id cellId.
		 * @param cellId Cell id
		 * @return Host set
		 */
		HostSet& getHostsInCell(cellid_t cellId) {
			return cellToHosts[cellId];
		}

	private:
		TbusCellShare(const TbusCellShare&);
		void operator=(const TbusCellShare&);

		/**
		 * Maps a cell id to the number of connected hosts.
		 */
		std::map<cellid_t, HostSet> cellToHosts;

	public:
		/**
		 * Singleton instantiation for every model.
		 * @return A TbusCellShare of model T
		 */
		template<class T> static TbusCellShare* getInstance() {
			static TbusCellShare* instance = new T(); ///< Static local variable used for singleton cleanup

			return instance;
		}

		/**
		 * Update cell model.
		 * Host host changed cells between from and to.
		 * @param from Cell host was in
		 * @param to Cell host is in
		 * @param host Optional host reference
		 */
		void hostMoved(cellid_t from, cellid_t to, TbusHost* host) {
			if (from != TBUS_INVALID_CELLID) {
				cellToHosts[from].erase(host);
			}

			if (to != TBUS_INVALID_CELLID) {
				cellToHosts[to].insert(host);
			}
		}

		/**
		 * Makes every host in cell cellId adapt its queue values.
		 * @param cellId Cell id
		 */
		void cellActivityChanged(cellid_t cellId) {
			HostSet hosts = cellToHosts[cellId];
			HostSet::iterator it;

			for (it = hosts.begin(); it != hosts.end(); ++it) {
				(*it)->callback->adaptQueueValues(ALL);
			}
		}

		/**
		 * Adapt the given value in a new returned value to the current cell model.
		 * The optional parameter host can be used for additional information.
		 * @param cellId Cell to adapt on
		 * @param value Given TbusQueueDatarateValue
		 * @param host Optional host reference
		 * @return Adapted queue datarate value
		 */
		virtual TbusQueueDatarateValue* adaptDatarateValue(cellid_t cellId, TbusQueueDatarateValue* value, TbusHost* host = NULL) = 0;

		/**
		 * Adapt the given value in a new returned value to the current cell model.
		 * The optional parameter host can be used for additional information.
		 * @param cellId Cell to adapt on
		 * @param value Given TbusQueueDelayValue
		 * @param host Optional host reference
		 * @return Adapted queue delay value
		 */
		virtual TbusQueueDelayValue* adaptDelayValue(cellid_t cellId, TbusQueueDelayValue* value, TbusHost* host = NULL) = 0;
};

#endif /* TBUSCELLSHARE_H_ */

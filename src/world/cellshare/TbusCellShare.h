/*
 * TbusCellShare.h
 *
 *  Created on: 17.02.2015
 *      Author: bialon
 */

#ifndef TBUSCELLSHARE_H_
#define TBUSCELLSHARE_H_

#include "omnetpp.h"
#include "TbusQueueValue.h"
#include "TbusCellShareTypes.h"

#include <map>
#include <set>
#include <algorithm>

struct CallbackTrigger {
	void operator() (TbusHost* host) {
		host->callback->triggerQueueValueUpdate(ALL);
	}
};

struct HostsTrigger {
	void operator() (std::pair<cellid_t, std::set<TbusHost*> > pair) {
		std::set<TbusHost*>& hosts = pair.second;
		CallbackTrigger callbackTrigger;

		std::for_each(hosts.begin(), hosts.end(), callbackTrigger);
	}
};

/**
 * Representation of a TBUS cell share module.
 */
class TbusCellShare {
	protected:
		/**
		 * Protected constructor for inheriting.
		 */
		TbusCellShare() {};

		typedef std::set<TbusHost*> hostSet;

		/**
		 * Returns the number of active hosts in the given cell.
		 * @param cellId Cell id of cell
		 * @return Number of active hosts
		 */
		uint64_t getActiveHostsInCell(cellid_t cellId) {
			hostSet::iterator it;
			uint64_t numActive = 0;

			for (it = cellToHosts[cellId].begin(); it != cellToHosts[cellId].end(); ++it) {
				if ((*it)->callback->isActive()) {
					numActive++;
				}
			}

			return numActive;
		}

	private:
		TbusCellShare(const TbusCellShare&);
		void operator=(const TbusCellShare&);

		/**
		 * Maps a cell id to the number of connected hosts.
		 */
		std::map<cellid_t, hostSet> cellToHosts;

		uint64_t registeredHosts;
		uint64_t currenUpdatedHosts;

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
		 * Register host at the cellshare model.
		 */
		void registerHost() {
			registeredHosts++;
		}

		/**
		 * Unregister host from the cellshare model.
		 */
		void unregisterHost() {
			ASSERT2(registeredHosts > 0, "Invalid unregisterHost: All previously registered hosts have already been unregistered!");

			registeredHosts--;
		}

		/**
		 * Update cell model.
		 * Host host changed cells between from and to.
		 * @param from Cell host was in
		 * @param to Cell host is in
		 * @param host Optional host reference
		 */
		void hostMoved(cellid_t from, cellid_t to, TbusHost* host) {
			currenUpdatedHosts++;

			if (from != TBUS_INVALID_CELLID) {
				cellToHosts[from].erase(host);
			}

			if (to != TBUS_INVALID_CELLID) {
				cellToHosts[to].insert(host);
			}

			if (currenUpdatedHosts == registeredHosts) {
				HostsTrigger hostsTrigger;
				// One round completed, trigger update Hosts
				std::for_each(cellToHosts.begin(), cellToHosts.end(), hostsTrigger);
			}
		}

		/**
		 * Adapt the given value to the current cell model.
		 * The optional parameter host can be used for additional information.
		 * @param cellId Cell to adapt on
		 * @param value Given TbusQueueValue
		 * @param host Optional host reference
		 */
		virtual void adaptValue(cellid_t cellId, TbusQueueValue* value, TbusHost* host = NULL) = 0;
};

#endif /* TBUSCELLSHARE_H_ */

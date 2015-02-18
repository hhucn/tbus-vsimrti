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
#include "TbusCallbackTarget.h"

#include <set>

typedef int64_t cellid_t;
#define TBUS_INVALID_CELLID (cellid_t) -1

class TbusQueueControl;

/**
 * Representation of a TBUS cell share module.
 */
class TbusCellShare {
	private:
		TbusCellShare(const TbusCellShare&);
		void operator=(const TbusCellShare&);

	protected:
		/**
		 * Protected constructor for inheriting.
		 */
		TbusCellShare() {};

		/**
		 * Callback set.
		 */
		std::set<TbusCallbackTarget::TbusCallback<TbusQueueControl>*> callbacks;
		/**
		 * Number of registered hosts.
		 */
		uint64_t numRegisteredHosts;

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
		 * Register a host.
		 * The optional parameter host can be used for additional information.
		 * IMPORTANT: Call this implementation if you override it first!
		 * @param host Optional host reference
		 */
		virtual void registerHost(cModule* host = NULL) {
			numRegisteredHosts++;
		}

		/**
		 * Unregister a host.
		 * The optional parameter host can be used for additional information.
		 * IMPORTANT: Call this implementation if you override it first!
		 * @param host Optional host reference
		 */
		virtual void unregisterHost(cModule* host = NULL) {
			ASSERT2(numRegisteredHosts > 0, "Invalid unregister of host - not enough registered hosts!");

			numRegisteredHosts--;
		}

		/**
		 * Adds a callback to the callback set.
		 * @param cb Callback to add
		 */
		template <class T> void addCallback(TbusCallbackTarget::TbusCallback<T>* cb) {
			callbacks.insert(cb);
		}

		/**
		 * Update cell model.
		 * Host host changed cells between from and to.
		 * IMPORTANT: Call this implementation if you override it last!
		 * @param from Cell host was in
		 * @param to Cell host is in
		 * @param host Optional host reference
		 */
		virtual void hostMoved(cellid_t from, cellid_t to, cModule* host = NULL) {
			if (callbacks.size() == numRegisteredHosts) {
				std::set<TbusCallbackTarget::TbusCallback<TbusQueueControl>*>::iterator it;

				for (it = callbacks.begin(); it != callbacks.end(); ++it) {
					(*it)->doCall();
					delete *it;
				}

				// Clear all entries
				callbacks.clear();
			}
		}

		/**
		 * Adapt the given value to the current cell model.
		 * The optional parameter host can be used for additional information.
		 * @param cellId Cell to adapt on
		 * @param value Given TbusQueueValue
		 * @param host Optional host reference
		 */
		virtual void adaptValue(cellid_t cellId, TbusQueueValue* value, cModule* host = NULL) = 0;
};

#endif /* TBUSCELLSHARE_H_ */

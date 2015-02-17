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

#include <typeinfo>

typedef int64_t cellid_t;

/**
 * Representation of a TBUS cell share module.
 */
class TbusCellShare {
	private:
		TbusCellShare(const TbusCellShare&);

	protected:
		/**
		 * Protected constructor for inheriting.
		 */
		explicit TbusCellShare() {};

	public:
		/**
		 * Singleton instantiation for every model.
		 * @return A TbusCellShare of model T
		 */
		template<class T> static TbusCellShare& getInstance() {
			static TbusCellShare instance = T(); ///< Static local variable used for singleton cleanup

			return instance;
		}

		/**
		 * Register a host for cell cellId.
		 * The optional parameter host can be used for additional information.
		 * @param cellId Cell to register on
		 * @param host Optional host reference
		 */
		virtual void registerHost(cellid_t cellId, cModule* host = NULL);
		/**
		 * Unregister a host from a cell.
		 * The optional parameter host can be used for additional information.
		 * @param cellId Cell to unregister from
		 * @param host Optional host reference
		 */
		virtual void unregisterHost(cellid_t cellId, cModule* host = NULL);

		/**
		 * Adapt the given value to the current cell model.
		 * The optional parameter host can be used for additional information.
		 * @param cellId Cell to adapt on
		 * @param value Given TbusQueueValue
		 * @param host Optional host reference
		 */
		virtual void adaptValue(cellid_t cellId, TbusQueueValue* value, cModule* host = NULL);
};

#endif /* TBUSCELLSHARE_H_ */

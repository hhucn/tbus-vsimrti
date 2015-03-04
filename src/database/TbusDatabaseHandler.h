/*
 * DatabaseHandler.h
 *
 *  Created on: 13.10.2014
 *      Author: bialon
 */

#ifndef DATABASEHANDLER_H_
#define DATABASEHANDLER_H_

#include "TbusQueueDatarateValue.h"
#include "TbusQueueDelayValue.h"
#include "TbusCellShareTypes.h"
#include "omnetpp.h"


/**
 * Abstract base class for all database handlers to be used in TBUS.
 * Provides an interface for accessing data-/droprate and delay from data source.
 */
class TbusDatabaseHandler {
	private:
		/**
		 * Private copy constructor
		 * @param
		 */
		TbusDatabaseHandler(const TbusDatabaseHandler&);
		void operator=(const TbusDatabaseHandler&);

	protected:
		/**
		 * Protected Constructor.
		 * Only inheriting is possible.
		 */
		TbusDatabaseHandler() {};

	public:
		/**
		 * Inline template function for instantiation of a singleton with given type T.
		 * This is used for an easy interface throughout the using classes and easy instantiation and maintenance of the belonging data source.
		 * @return Singleton instance of type T
		 */
		template<class T> static TbusDatabaseHandler* getInstance() {
			static TbusDatabaseHandler* instance = new T(); ///< static local variable used for singleton cleanup

			return instance;
		}

		/**
		 * Destructor.
		 * Database cleanup, query finalization and database closing should be handled here by derived classes.
		 */
		virtual ~TbusDatabaseHandler() {};

		/**
		 * Return the cellid at position roadId and lanePos
		 * @param roadId Current road id
		 * @param lanePos Lane position
		 * @return Cell id
		 */
		virtual cellid_t getCellId(const char* const roadId, const float lanePos, simtime_t time = simTime()) = 0;

		virtual TbusQueueDatarateValue* getUploadDatarate(const char* const roadId, const float lanePos, simtime_t time = simTime()) = 0;
		virtual TbusQueueDelayValue* getUploadDelay(const char* const roadId, const float lanePos, simtime_t time = simTime()) = 0;
		virtual TbusQueueDatarateValue* getDownloadDatarate(const char* const roadId, const float lanePos, simtime_t time = simTime()) = 0;
		virtual TbusQueueDelayValue* getDownloadDelay(const char* const roadId, const float lanePos, simtime_t time = simTime()) = 0;
};

#endif /* DATABASEHANDLER_H_ */

/*
 * DatabaseHandler.h
 *
 *  Created on: 13.10.2014
 *      Author: bialon
 */

#ifndef DATABASEHANDLER_H_
#define DATABASEHANDLER_H_

#include <stdlib.h>
#include <iostream>
#include <typeinfo>

#include "TbusQueueDatarateValue.h"
#include "TbusQueueDelayValue.h"
#include "Coord.h"
#include "omnetpp.h"

#include <set>

/**
 * Abstract base class for all database handlers to be used in TBUS.
 * Provides an interface for accessing data-/droprate and delay from data source.
 */
class DatabaseHandler {
	private:
		/**
		 * Private copy constructor
		 * @param
		 */
		DatabaseHandler(const DatabaseHandler&);

		std::set<TbusQueueControl*> callbackHandlers;
		uint64_t numRegisteredCallbackHandlers = 0;

	protected:
		/**
		 * Protected Constructor.
		 * Only inheriting is possible.
		 */
		explicit DatabaseHandler() {};

	public:
		/**
		 * Inline template function for instantiation of a singleton with given type T.
		 * This is used for an easy interface throughout the using classes and easy instantiation and maintenance of the belonging data source.
		 * @return Singleton instance of type T
		 */
		template<class T> static DatabaseHandler& getInstance() {
			//		static DatabaseHandler* instance = NULL; ///< static singleton instance. Used for guaranteed memory cleanup.
			//
			//		if (!instance) {
			//			instance = dynamic_cast<DatabaseHandler*>(new T());
			//
			//			if (!instance) {
			//				std::cerr << __FILE__ << ":" << __FUNCTION__ << " (" << __LINE__ << ") ERROR! Class DatabaseHandler is no base class of class \"" << typeid(T).name() << "\" ! Aborting..." << std::endl;
			//				exit(1);
			//			}
			//		}
			static DatabaseHandler instance = T(); ///< static local variable used for singleton cleanup

			return instance;
		}

		/**
		 * Destructor.
		 * Database cleanup, query finalization and database closing should be handled here by derived classes.
		 */
		virtual ~DatabaseHandler();

		/**
		 * Increase the number of registered callback handlers: Register a host
		 */
		void increaseCallbackHandlers() {
			numRegisteredCallbackHandlers++;
		}

		/**
		 * Decrease the number of registered callback handlers: Unregister a host
		 */
		void decreaseCallbackHandlers() {
			ASSERT2(numRegisteredCallbackHandlers > 0, "Invalid decrease of registered callback handlers!");

			numRegisteredCallbackHandlers--;
		}

		cellid_t getCellId(const char* const roadId, const float lanePos, TbusQueueControl* callbackHandler) {
			callbackHandlers.insert(callbackHandler);

			//TODO: Get cell id from database

			// If every callback handler got its cell id, do the callbacks
			if (callbackHandlers.size() == numRegisteredCallbackHandlers) {
				doCallbacks();

				// And clear the list
				callbackHandlers.clear();
			}
		}

		void doCallbacks() {
			std::set<TbusQueueControl*>::iterator it;
			for (it = callbackHandlers.begin(); it != callbackHandlers.end(); ++it) {
				(*it)->cellUpdateCompleteCallback();
			}
		}

		/**
		 * Return the upload data- and droprate at position pos and time time.
		 * @param pos Position to look at
		 * @param time Time to look at or current time
		 * @return Data- and droprate belonging to pos and time
		 */
		virtual TbusQueueDatarateValue* getUploadDatarate(const Coord& pos, simtime_t time = simTime());

		/**
		 * Return the upload delay at position pos and time time.
		 * @param pos Position to look at
		 * @param time Time to look at or current time
		 * @return Delay belonging to pos and time
		 */
		virtual TbusQueueDelayValue* getUploadDelay(const Coord& pos, simtime_t time = simTime());

		/**
		 * Return the download data- and droprate at position pos and time time.
		 * @param pos Position to look at
		 * @param time Time to look at or current time
		 * @return Data- and droprate belonging to pos and time
		 */
		virtual TbusQueueDatarateValue* getDownloadDatarate(const Coord& pos, simtime_t time = simTime());

		/**
		 * Return the download delay at position pos and current simulation time.
		 * @param pos Position to look at or current time
		 * @return Delay belonging to pos and current simulation time
		 */
		virtual TbusQueueDelayValue* getDownloadDelay(const Coord& pos, simtime_t time = simTime());

		virtual TbusQueueDatarateValue* getUploadDatarate(const char* const roadId, const float lanePos, simtime_t time = simTime());
		virtual TbusQueueDelayValue* getUploadDelay(const char* const roadId, const float lanePos, simtime_t time = simTime());
		virtual TbusQueueDatarateValue* getDownloadDatarate(const char* const roadId, const float lanePos, simtime_t time = simTime());
		virtual TbusQueueDelayValue* getDownloadDelay(const char* const roadId, const float lanePos, simtime_t time = simTime());
};

#endif /* DATABASEHANDLER_H_ */

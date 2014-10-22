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

/**
 * Abstract base class for all database handlers to be used in TBUS.
 * Provides an interface for accessing data-/droprate and delay from data source.
 */
class DatabaseHandler {
protected:
		/**
		 * Constructor.
		 * Opening and sanity-checking the database should be handled here for all derived classes.
		 */
		explicit DatabaseHandler() {};

public:
	/**
	 * Inline template function for instantiation of a singleton with given type T.
	 * This is used for an easy interface throughout the using classes and easy instantiation and maintenance of the belonging data source.
	 * @return Singleton instance of type T
	 */
	template<class T> static DatabaseHandler* getInstance() {
		static DatabaseHandler* instance = 0; ///< static singleton instance. Used for guaranteed memory cleanup.

		if (!instance) {
			instance = dynamic_cast<DatabaseHandler*>(new T());

			if (!instance) {
				std::cerr << __FILE__ << ":" << __FUNCTION__ << " (" << __LINE__ << ") ERROR! Class DatabaseHandler is no base class of class \"" << typeid(T).name() << "\" ! Aborting..." << std::endl;
				exit(1);
			}
		}

		return instance;
	}

	/**
	 * Destructor.
	 * Database cleanup, query finalization and database closing should be handled here by derived classes.
	 */
    virtual ~DatabaseHandler() {};

    /**
     * Return the upload data- and droprate at position pos and time time.
     * @param pos Position to look at
     * @param time Time to look at
     * @return Data- and droprate belonging to pos and time
     */
    virtual TbusQueueDatarateValue* getUploadDatarate(const Coord& pos, simtime_t time) = 0;
    /**
     * Return the upload data- and droprate at position pos and current simulation time.
     * @param pos Position to look at
     * @return Data- and droprate belonging to pos and current simulation time
     */
    virtual TbusQueueDatarateValue* getUploadDatarate(const Coord& pos) = 0;

    /**
     * Return the upload delay at position pos and time time.
     * @param pos Position to look at
     * @param time Time to look at
     * @return Delay belonging to pos and time
     */
    virtual TbusQueueDelayValue* getUploadDelay(const Coord& pos, simtime_t time) = 0;
    /**
     * Return the upload delay at position pos and current simulation time.
     * @param pos Position to look at
     * @return Delay belonging to pos and current simulation time
     */
    virtual TbusQueueDelayValue* getUploadDelay(const Coord& pos) = 0;

    /**
     * Return the download data- and droprate at position pos and time time.
     * @param pos Position to look at
     * @param time Time to look at
     * @return Data- and droprate belonging to pos and time
     */
    virtual TbusQueueDatarateValue* getDownloadDatarate(const Coord& pos, simtime_t time) = 0;
    /**
     * Return the download data- and droprate at position pos and current simulation time.
     * @param pos Position to look at
     * @return Data- and droprate belonging to pos and current simulation time
     */
    virtual TbusQueueDatarateValue* getDownloadDatarate(const Coord& pos) = 0;

    /**
     * Return the download delay at position pos and time time.
     * @param pos Position to look at
     * @param time Time to look at
     * @return Delay belonging to pos and time
     */
    virtual TbusQueueDelayValue* getDownloadDelay(const Coord& pos, simtime_t time) = 0;
    /**
     * Return the download delay at position pos and current simulation time.
     * @param pos Position to look at
     * @return Delay belonging to pos and current simulation time
     */
    virtual TbusQueueDelayValue* getDownloadDelay(const Coord& pos) = 0;
};

#endif /* DATABASEHANDLER_H_ */

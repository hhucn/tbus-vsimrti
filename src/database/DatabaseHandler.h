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

class DatabaseHandler {
protected:
		explicit DatabaseHandler() {};

public:
	template<class T> static DatabaseHandler* getInstance() {
		static DatabaseHandler* instance = 0;

		if (!instance) {
			instance = dynamic_cast<DatabaseHandler*>(new T());

			if (!instance) {
				std::cerr << __FILE__ << ":" << __FUNCTION__ << " (" << __LINE__ << ") ERROR! Class DatabaseHandler is no base class of class \"" << typeid(T).name() << "\" ! Aborting..." << std::endl;
				exit(1);
			}
		}

		return instance;
	}

    virtual ~DatabaseHandler() {};

    virtual TbusQueueDatarateValue* getDatarate(Coord& pos, simtime_t time) = 0;
    virtual TbusQueueDatarateValue* getDatarate(Coord& pos) = 0;

    virtual TbusQueueDelayValue* getDelay(Coord& pos, simtime_t time) = 0;
    virtual TbusQueueDelayValue* getDelay(Coord& pos) = 0;
};

#endif /* DATABASEHANDLER_H_ */

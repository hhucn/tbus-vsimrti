/*
 * SqliteDatabaseHandler.h
 *
 *  Created on: 13.10.2014
 *      Author: bialon
 */

#ifndef SQLITEDATABASEHANDLER_H_
#define SQLITEDATABASEHANDLER_H_

#include <sqlite3.h>
#include "DatabaseHandler.h"

class SqliteDatabaseHandler : public DatabaseHandler {
	private:
		sqlite3* database;


		static int callbackHandler(SqliteDatabaseHandler* handler, int numCols, char** colValues, char** colNames);
		inline int callback(int numCols, char** colValues, char** colNames);

		int32_t getDatabaseVersion();

	public:
		SqliteDatabaseHandler();
		virtual ~SqliteDatabaseHandler();

	    virtual TbusQueueDatarateValue* getDatarate(Coord& pos, simtime_t time);
	    virtual TbusQueueDatarateValue* getDatarate(Coord& pos);

	    virtual TbusQueueDelayValue* getDelay(Coord& pos, simtime_t time);
	    virtual TbusQueueDelayValue* getDelay(Coord& pos);
};

#endif /* SQLITEDATABASEHANDLER_H_ */

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

#define SQLITE_CALLBACK_PARAMS int numCols, char** colValues, char** colNames

class SqliteDatabaseHandler : public DatabaseHandler {
	private:
		sqlite3* database;

		sqlite3_stmt* uploadDatarateStatement;
		sqlite3_stmt* uploadDelayStatement;
		sqlite3_stmt* downloadDatarateStatement;
		sqlite3_stmt* downloadDelayStatement;

		static int uploadCallbackHandler(SqliteDatabaseHandler* handler, SQLITE_CALLBACK_PARAMS);
		inline int uploadCallback(SQLITE_CALLBACK_PARAMS);

		static int downloadCallbackHandler(SqliteDatabaseHandler* handler, SQLITE_CALLBACK_PARAMS);
		inline int downloadCallback(SQLITE_CALLBACK_PARAMS);

		int32_t getDatabaseVersion();

		inline void abort();

	public:
		SqliteDatabaseHandler();
		virtual ~SqliteDatabaseHandler();

	    virtual TbusQueueDatarateValue* getUploadDatarate(const Coord& pos, simtime_t time);
	    virtual TbusQueueDatarateValue* getUploadDatarate(const Coord& pos);

	    virtual TbusQueueDelayValue* getUploadDelay(const Coord& pos, simtime_t time);
	    virtual TbusQueueDelayValue* getUploadDelay(const Coord& pos);

	    virtual TbusQueueDatarateValue* getDownloadDatarate(const Coord& pos, simtime_t time);
	    virtual TbusQueueDatarateValue* getDownloadDatarate(const Coord& pos);

	    virtual TbusQueueDelayValue* getDownloadDelay(const Coord& pos, simtime_t time);
	    virtual TbusQueueDelayValue* getDownloadDelay(const Coord& pos);
};

#endif /* SQLITEDATABASEHANDLER_H_ */

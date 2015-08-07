/*
 * SqliteDatabaseHandler.h
 *
 *  Created on: 13.10.2014
 *      Author: bialon
 */

/**
 * @file SqliteDatabaseHandler.h
 * Header file of the SQLite database handler
 */

#ifndef SQLITEDATABASEHANDLER_H_
#define SQLITEDATABASEHANDLER_H_

#include <sqlite3.h>
#include "TbusDatabaseHandler.h"

/**
 * SQLite user_version database version for version check
 */
#define TBUS_SQLITE_USER_VERSION	0x00000001
/**
 * KB to Bit conversion, used by multiplication
 */
#define TBUS_KBYTE_TO_BIT			(1024.0 * 8.0)
/**
 * Error macro if query returns anything but a row
 */
#define TBUS_NOTAROW(FILE,LINE)		std::cerr << FILE << " (" << LINE << "): Unable to retrieve values from database - query result is not a row!" << std::endl << "SQLite error " << sqlite3_errcode(database) << ": " << sqlite3_errmsg(database) << std::endl;

/**
 * Default datarate value if no value could be retrieved (In Bit/s)
 */
#define TBUS_DATARATE_DEFAULT		1ll
/**
 * Default droprate value if no value could be retrieved (In percent)
 */
#define TBUS_DROPRATE_DEFAULT		1.0
/**
 * Default delay value if no value could be retrieved (in ns)
 */
#define TBUS_DELAY_DEFAULT			1ll
/**
 * Default cell id value if no value could be retrieved
 */
#define TBUS_CELLID_DEFAULT			0
/**
 * Default group id value if no value could be retrieved
 */
#define TBUS_GROUPID_DEFAULT		1

/**
 * @class TbusSqliteDatabaseHandler
 * Database handler for SQLite database.
 * For edge based SQL database structure, see tbus_edge.sql or below
 *
 * @code
 * CREATE TABLE upload_datarate (
 *		id INTEGER PRIMARY KEY,
 *		timestamp INTEGER NOT NULL,
 *		roadId TEXT,
 *		lanePos REAL,
 *		cellId INTEGER,
 *		datarate REAL,
 *		droprate REAL
 * );
 *
 * CREATE TABLE download_datarate (
 *		id INTEGER PRIMARY KEY,
 *		timestamp INTEGER NOT NULL,
 *		roadId TEXT,
 *		lanePos REAL,
 *		cellId INTEGER,
 *		datarate REAL,
 *		droprate REAL
 * );
 *
 *CREATE TABLE upload_delay (
 *		id INTEGER PRIMARY KEY,
 *		timestamp INTEGER NOT NULL,
 *		roadId TEXT,
 *		lanePos REAL,
 *		delay INTEGER
 * );
 *
 * CREATE TABLE download_delay (
 *		id INTEGER PRIMARY KEY,
 *		timestamp INTEGER NOT NULL,
 *		roadId TEXT,
 *		lanePos REAL,
 *		delay INTEGER
 * );
 * @endcode
 */
class TbusSqliteDatabaseHandler : public TbusDatabaseHandler {
	friend class TbusDatabaseHandler;
	private:
		sqlite3* database; ///< Database connection

		sqlite3_stmt* uploadDatarateStatementEdge; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.
		sqlite3_stmt* uploadDelayStatementEdge; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.
		sqlite3_stmt* downloadDatarateStatementEdge; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.
		sqlite3_stmt* downloadDelayStatementEdge; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.

		sqlite3_stmt* uploadCellIdStatementEdge; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.
		sqlite3_stmt* downloadCellIdStatementEdge; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.

		sqlite3_stmt* uploadGroupIdRestricted; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.
		sqlite3_stmt* uploadGroupIdUnrestricted; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.

		sqlite3_stmt* downloadGroupIdRestricted; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.
		sqlite3_stmt* downloadGroupIdUnrestricted; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.

		int32_t getDatabaseVersion();

		char* getRoadIdSubstring(const char* const roadId);

		bool executeGroupIdQuery(sqlite3_stmt* query, const char* const roadId, simtime_t time);

		TbusQueueDatarateValue* getDatarateValue(sqlite3_stmt* query, const char* const roadId, const float lanePos, uint64_t groupId);
		TbusQueueDelayValue* getDelayValue(sqlite3_stmt* query, const char* const roadId, const float lanePos, uint64_t groupId);

		inline void abort();

	protected:
		TbusSqliteDatabaseHandler();

	public:
		virtual ~TbusSqliteDatabaseHandler();

		virtual cellid_t getCellId(const char* const roadId, const float lanePos, simtime_t time = simTime());

		virtual uint64_t getUploadGroupId(const char* const roadId, simtime_t time = simTime());
		virtual uint64_t getDownloadGroupId(const char* const roadId, simtime_t time = simTime());

	    // Edge based
	    virtual TbusQueueDatarateValue* getUploadDatarate(const char* const roadId, const float lanePos, simtime_t time = simTime());
	    virtual TbusQueueDelayValue* getUploadDelay(const char* const roadId, const float lanePos, simtime_t time = simTime());
	    virtual TbusQueueDatarateValue* getDownloadDatarate(const char* const roadId, const float lanePos, simtime_t time = simTime());
	    virtual TbusQueueDelayValue* getDownloadDelay(const char* const roadId, const float lanePos, simtime_t time = simTime());
};

#endif /* SQLITEDATABASEHANDLER_H_ */

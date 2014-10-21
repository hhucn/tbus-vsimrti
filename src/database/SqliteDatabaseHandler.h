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
#include "DatabaseHandler.h"

/**
 * SQLite database file name
 */
#define TBUS_SQLITE_DATABASE "test.sqlite"
/**
 * SQLite user_version database version for version check
 */
#define TBUS_SQLITE_USER_VERSION 0x00000001
/**
 * MBit to Bit conversion, used by multiplication
 */
#define TBUS_MBIT_TO_BIT	(1024*1024)
/**
 * nanosecond to second conversion, used by multiplication
 */
#define TBUS_NSEC_TO_SEC	10e-9
/**
 * Error macro if query returns anything but a row
 */
#define TBUS_NOTAROW(FILE,LINE) std::cerr << FILE << " (" << LINE << "): Unable to retrieve values from database - query result is not a row!" << std::endl << "SQLite error " << sqlite3_errcode(database) << ": " << sqlite3_errmsg(database) << std::endl;

/**
 * @class SqliteDatabaseHandler
 * Database handler for SQLite database. For SQL database structure, see tbus.sql or below.
 * @code
 *     SQLite database structure:
 *
 *      CREATE TABLE upload_datarate (
 *     		id INTEGER PRIMARY KEY,
 *     		latitude REAL NOT NULL,
 *     		longitude REAL NOT NULL,
 *     		timestamp INTEGER NOT NULL,
 *     		datarate REAL,
 *     		droprate REAL
 *     	);
 *
 *     	CREATE TABLE download_datarate (
 *     		id INTEGER PRIMARY KEY,
 *     		latitude REAL NOT NULL,
 *     		longitude REAL NOT NULL,
 *     		timestamp INTEGER NOT NULL,
 *     		datarate REAL,
 *     		droprate REAL
 *     	);
 *
 *     	CREATE TABLE upload_delay (
 *     		id INTEGER PRIMARY KEY,
 *     		latitude REAL NOT NULL,
 *     		longitude REAL NOT NULL,
 *     		timestamp INTEGER NOT NULL,
 *     		 delay REAL
 *     	);
 *
 *     	CREATE TABLE download_delay (
 *     		id INTEGER PRIMARY KEY,
 *     		latitude REAL NOT NULL,
 *     		longitude REAL NOT NULL,
 *     		timestamp INTEGER NOT NULL,
 *     		delay REAL
 *     	);
 * @endcode
 */
class SqliteDatabaseHandler : public DatabaseHandler {
	private:
		sqlite3* database; ///< Database connection

		sqlite3_stmt* uploadDatarateStatement; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.
		sqlite3_stmt* uploadDelayStatement; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.
		sqlite3_stmt* downloadDatarateStatement; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.
		sqlite3_stmt* downloadDelayStatement; ///< Prepared SQL statement for memory efficiency. Values are re-binded before evaluation.

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

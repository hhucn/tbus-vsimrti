/*
 * SqliteDatabaseHandler.cc
 *
 *  Created on: 13.10.2014
 *      Author: Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#include "SqliteDatabaseHandler.h"

/**
 * Creates a new database connection by opening TBUS_SQLITE_DATABASE and checks the database version.
 * Aborts on error with exit code 1 and message on stderr and closes the database connection.
 */
SqliteDatabaseHandler::SqliteDatabaseHandler() {
	int result = 0;

	result = sqlite3_open(TBUS_SQLITE_DATABASE, &database);

	if (result) {
		std::cerr << "Unable to open database " << TBUS_SQLITE_DATABASE << "! Exiting..." << std::endl;
		this->abort();
	}

	// Check databases' user_version for compatibility
	int32_t version = getDatabaseVersion();
	if (version != TBUS_SQLITE_USER_VERSION) {
		std::cerr << "Expected database version " << TBUS_SQLITE_USER_VERSION << ", but found version "<< version << "! Exiting..." << std::endl;
		this->abort();
	}
}

/**
 * Finalizes prepared statements and closes database connection.
 */
SqliteDatabaseHandler::~SqliteDatabaseHandler() {
	sqlite3_finalize(uploadDatarateStatement);
	sqlite3_finalize(uploadDelayStatement);
	sqlite3_finalize(downloadDatarateStatement);
	sqlite3_finalize(downloadDelayStatement);

	sqlite3_close(database);
}

/**
 * Retrieves the database version as of SQLites' pragma user_version 32-bit int field.
 * @return The database version
 */
int32_t SqliteDatabaseHandler::getDatabaseVersion() {
	sqlite3_stmt* statement;

	sqlite3_prepare_v2(database, "pragma user_version;", -1, &statement, NULL);
	if (sqlite3_step(statement) != SQLITE_ROW) {
		std::cerr << "Unable to retrieve database version! Exiting..." << std::endl;
		this->abort();
	}

	int32_t version = sqlite3_column_int(statement, 0);
	sqlite3_finalize(statement);

	return version;
}

/**
 * Aborts execution by closing the database handler and exiting with value 1
 */
void SqliteDatabaseHandler::abort() {
	sqlite3_close(database);
	exit(1);
}

/**
 * Retrieves upload data- and droprate for position pos and at time time. Both values are chosen from the database by looking at the smallest distance
 * on the two-dimensional position plane and one-dimensional timeline.
 * The returned object has to be destroyed by the caller.
 * @param pos The position to look at
 * @param time The time to look at
 * @return Data- and droprate as a TbusQueueDatarateValue object
 */
TbusQueueDatarateValue* SqliteDatabaseHandler::getUploadDatarate(const Coord& pos, simtime_t time) {
	TbusQueueDatarateValue* result = new TbusQueueDatarateValue();

	static bool once = true;
	if (once) {
		// Only prepare once
		sqlite3_prepare_v2(database, "select datarate, droprate, min((latitude - ?1)*(latitude - ?1) + (longitude - ?2)*(longitude - ?2)), min(abs(timestamp - ?3)) from upload_datarate;", -1 , &uploadDatarateStatement, NULL);
		once = false;
	}

	sqlite3_reset(uploadDatarateStatement);
	sqlite3_bind_double(uploadDatarateStatement, 1, pos.x);
	sqlite3_bind_double(uploadDatarateStatement, 2, pos.y);
	sqlite3_bind_int(uploadDatarateStatement, 3, time.inUnit(SIMTIME_NS));

	if (sqlite3_step(uploadDatarateStatement) != SQLITE_ROW) {
		TBUS_NOTAROW(__FILE__, __LINE__)
		// Set dummy values
		result->datarate = 0.0;
		result->droprate = 0.0;
	} else {
		// Retrieve values from database
		result->datarate = sqlite3_column_double(uploadDatarateStatement, 0) * TBUS_MBIT_TO_BIT;
		result->droprate = sqlite3_column_double(uploadDatarateStatement, 1);
	}

	return result;
}

/**
 * Retrieves upload data- and droprate for position pos at the current simTime()
 * @see SqliteDatabaseHandler::getUploadDatarate(const Coord&, simtime_t)
 * @param pos The position to look at
 * @return Data- and droprate as a TbusQueueDatarateValue object
 */
TbusQueueDatarateValue* SqliteDatabaseHandler::getUploadDatarate(const Coord& pos) {
	return this->getUploadDatarate(pos, simTime());
}

/**
 * Retrieves upload delay for position pos and at time time. Both values are chosen from the database by looking at the smallest distance
 * on the two-dimensional position plane and one-dimensional timeline.
 * The returned object has to be destroyed by the caller.
 * @param pos The position to look at
 * @param time The time to look at
 * @return Delay as a TbusQueueDelayValue object
 */
TbusQueueDelayValue* SqliteDatabaseHandler::getUploadDelay(const Coord& pos, simtime_t time) {
	TbusQueueDelayValue* result = new TbusQueueDelayValue();

	static bool once = true;
	if (once) {
		// Only prepare once
		sqlite3_prepare_v2(database, "select delay, min((latitude - ?1)*(latitude - ?1) + (longitude - ?2)*(longitude - ?2)), min(abs(timestamp - ?3)) from upload_delay;", -1 , &uploadDelayStatement, NULL);
		once = false;
	}

	sqlite3_reset(uploadDelayStatement);
	sqlite3_bind_double(uploadDelayStatement, 1, pos.x);
	sqlite3_bind_double(uploadDelayStatement, 2, pos.y);
	sqlite3_bind_int(uploadDelayStatement, 3, time.inUnit(SIMTIME_NS));

	if (sqlite3_step(uploadDelayStatement) != SQLITE_ROW) {
		TBUS_NOTAROW(__FILE__, __LINE__)
		// Set dummy values
		result->delay = 0.0;
	} else {
		// Retrieve values from database
		result->delay = sqlite3_column_double(uploadDelayStatement, 0) * TBUS_NSEC_TO_SEC;
	}

	return result;
}

/**
 * Retrieves upload delay for position pos at the current simTime()
 * @see SqliteDatabaseHandler::getUploadDelay(const Coord&, simtime_t)
 * @param pos The position to look at
 * @return Delay as a TbusQueueDelayValue object
 */
TbusQueueDelayValue* SqliteDatabaseHandler::getUploadDelay(const Coord& pos) {
	return this->getUploadDelay(pos, simTime());
}

/**
 * Retrieves download data- and droprate for position pos and at time time. Both values are chosen from the database by looking at the smallest distance
 * on the two-dimensional position plane and one-dimensional timeline.
 * The returned object has to be destroyed by the caller.
 * @param pos The position to look at
 * @param time The time to look at
 * @return Data- and droprate as a TbusQueueDatarateValue object
 */
TbusQueueDatarateValue* SqliteDatabaseHandler::getDownloadDatarate(const Coord& pos, simtime_t time) {
	TbusQueueDatarateValue* result = new TbusQueueDatarateValue();

	static bool once = true;
	if (once) {
		// Only prepare once
		sqlite3_prepare_v2(database, "select datarate, droprate, min((latitude - ?1)*(latitude - ?1) + (longitude - ?2)*(longitude - ?2)), min(abs(timestamp - ?3)) from download_datarate;", -1 , &downloadDatarateStatement, NULL);
		once = false;
	}

	sqlite3_reset(downloadDatarateStatement);
	sqlite3_bind_double(downloadDatarateStatement, 1, pos.x);
	sqlite3_bind_double(downloadDatarateStatement, 2, pos.y);
	sqlite3_bind_int(downloadDatarateStatement, 3, time.inUnit(SIMTIME_NS));

	if (sqlite3_step(downloadDatarateStatement) != SQLITE_ROW) {
		TBUS_NOTAROW(__FILE__, __LINE__)
		// Set dummy values
		result->datarate = 0.0;
		result->droprate = 0.0;
	} else {
		// Retrieve values from database
		result->datarate = sqlite3_column_double(downloadDatarateStatement, 0) * TBUS_MBIT_TO_BIT;
		result->droprate = sqlite3_column_double(downloadDatarateStatement, 1);
	}

	return result;
}

/**
 * Retrieves download data- and droprate for position pos at the current simTime()
 * @see SqliteDatabaseHandler::getDownloadDatarate(const Coord&, simtime_t)
 * @param pos The position to look at
 * @return Data- and droprate as a TbusQueueDatarateValue object
 */
TbusQueueDatarateValue* SqliteDatabaseHandler::getDownloadDatarate(const Coord& pos) {
	return this->getDownloadDatarate(pos, simTime());
}

/**
 * Retrieves download delay for position pos and at time time. Both values are chosen from the database by looking at the smallest distance
 * on the two-dimensional position plane and one-dimensional timeline.
 * The returned object has to be destroyed by the caller.
 * @param pos The position to look at
 * @param time The time to look at
 * @return Delay as a TbusQueueDelayValue object
 */
TbusQueueDelayValue* SqliteDatabaseHandler::getDownloadDelay(const Coord& pos, simtime_t time) {
	TbusQueueDelayValue* result = new TbusQueueDelayValue();

	static bool once = true;
	if (once) {
		// Only prepare once
		sqlite3_prepare_v2(database, "select delay, min((latitude - ?1)*(latitude - ?1) + (longitude - ?2)*(longitude - ?2)), min(abs(timestamp - ?3)) from download_delay;", -1 , &downloadDelayStatement, NULL);
		once = false;
	}

	sqlite3_reset(downloadDelayStatement);
	sqlite3_bind_double(downloadDelayStatement, 1, pos.x);
	sqlite3_bind_double(downloadDelayStatement, 2, pos.y);
	sqlite3_bind_int(downloadDelayStatement, 3, time.inUnit(SIMTIME_NS));

	if (sqlite3_step(downloadDelayStatement) != SQLITE_ROW) {
		TBUS_NOTAROW(__FILE__, __LINE__)
		// Set dummy values
		result->delay = 0.0;
	} else {
		// Retrieve values from database
		result->delay = sqlite3_column_double(downloadDelayStatement, 0) * TBUS_NSEC_TO_SEC;
	}

	return result;
}

/**
 * Retrieves download delay for position pos at the current simTime()
 * @see SqliteDatabaseHandler::getDownloadDelay(const Coord&, simtime_t)
 * @param pos The position to look at
 * @return Delay as a TbusQueueDelayValue object
 */
TbusQueueDelayValue* SqliteDatabaseHandler::getDownloadDelay(const Coord& pos) {
	return this->getDownloadDelay(pos, simTime());
}

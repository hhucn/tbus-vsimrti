/*
 * TbusSqliteDatabaseHandler.cc
 *
 *  Created on: 13.10.2014
 *      Author: Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#include "TbusSqliteDatabaseHandler.h"
#include <string>
#include <sstream>
#include <iostream>

Register_GlobalConfigOption(CFGID_TBUS_DATABASE_FILE, "tbus-database", CFG_FILENAME, "", "Filename of TBUS database")

#define TBUS_VERBOSE

/**
 * Creates a new database connection by opening TBUS_SQLITE_DATABASE and checks the database version.
 * Aborts on error and closes the database connection.
 */
TbusSqliteDatabaseHandler::TbusSqliteDatabaseHandler() {
	int result = 0;
	database = NULL;
	const std::string databaseFile = ev.getConfig()->getAsFilename(CFGID_TBUS_DATABASE_FILE);

	result = sqlite3_open_v2(databaseFile.c_str(), &database, SQLITE_OPEN_READONLY, NULL);

	if (result != SQLITE_OK) {
		abort();
		throw cRuntimeError("Unable to open database %s", databaseFile.c_str());
	} else {
		EV << "Successfully opened database " << databaseFile << std::endl;
	}

	// Check databases' user_version for compatibility
	int32_t version = getDatabaseVersion();
	if (version != TBUS_SQLITE_USER_VERSION) {
		abort();
		throw cRuntimeError("Expected database version %d but found version %d!", TBUS_SQLITE_USER_VERSION, version);
	}

	uploadDatarateStatementEdge = NULL;
	uploadDelayStatementEdge = NULL;
	downloadDatarateStatementEdge = NULL;
	downloadDelayStatementEdge = NULL;

	// Edge based
//	// Upload datarate
//	sqlite3_prepare_v2(database, "select datarate, droprate, max(lanePos), max(timestamp) from upload_datarate where roadId = ?2 and lanePos <= ?1 and timestamp <= ?3 limit 1;", -1 , &uploadDatarateStatementEdge, NULL);
//	// Upload delay
//	sqlite3_prepare_v2(database, "select delay, max(lanePos), max(timestamp) from upload_delay where roadId = ?2 and lanePos <= ?1 and timestamp <= ?3 limit 1;", -1 , &uploadDelayStatementEdge, NULL);
//	// Download datarate
//	sqlite3_prepare_v2(database, "select datarate, droprate, max(lanePos), max(timestamp) from download_datarate where roadId = ?2 and lanePos <= ?1 and timestamp <= ?3 limit 1;", -1 , &downloadDatarateStatementEdge, NULL);
//	// Download delay
//	sqlite3_prepare_v2(database, "select delay, max(lanePos), max(timestamp) from download_delay where roadId = ?2 and lanePos <= ?1 and timestamp <= ?3 limit 1;", -1 , &downloadDelayStatementEdge, NULL);

	// Upload datarate
	sqlite3_prepare_v2(database, "select datarate, droprate, abs(lanePos - ?1) as dist from upload_datarate where roadId = ?2 and timestamp <= ?3 order by dist asc limit 1;", -1 , &uploadDatarateStatementEdge, NULL);
	// Upload delay
	sqlite3_prepare_v2(database, "select delay, abs(lanePos - ?1) as dist from upload_delay where roadId = ?2 and timestamp <= ?3 order by dist asc limit 1;", -1 , &uploadDelayStatementEdge, NULL);
	// Download datarate
	sqlite3_prepare_v2(database, "select datarate, droprate, abs(lanePos - ?1) as dist from download_datarate where roadId = ?2 and timestamp <= ?3 order by dist asc limit 1;", -1 , &downloadDatarateStatementEdge, NULL);
	// Download delay
	sqlite3_prepare_v2(database, "select delay, abs(lanePos - ?1) as dist from download_delay where roadId = ?2 and timestamp <= ?3 order by dist asc limit 1;", -1 , &downloadDelayStatementEdge, NULL);
}

/**
 * Finalizes prepared statements and closes database connection.
 */
TbusSqliteDatabaseHandler::~TbusSqliteDatabaseHandler() {
	// Edge based
	sqlite3_finalize(uploadDatarateStatementEdge);
	sqlite3_finalize(uploadDelayStatementEdge);
	sqlite3_finalize(downloadDatarateStatementEdge);
	sqlite3_finalize(downloadDelayStatementEdge);

	sqlite3_close(database);
}

/**
 * Retrieves the database version as of SQLites' pragma user_version 32-bit int field.
 * @return The database version
 */
int32_t TbusSqliteDatabaseHandler::getDatabaseVersion() {
	sqlite3_stmt* statement;

	sqlite3_prepare_v2(database, "pragma user_version;", -1, &statement, NULL);
	if (sqlite3_step(statement) != SQLITE_ROW) {
		std::cerr << "Unable to retrieve database version! Exiting..." << std::endl;
		abort();

		return -1;
	}

	int32_t version = sqlite3_column_int(statement, 0);
	sqlite3_finalize(statement);

	return version;
}

/**
 * Aborts execution by closing the database handler and exiting with value 1
 */
void TbusSqliteDatabaseHandler::abort() {
	sqlite3_close(database);
}

/**
 * Creates a substring copy of the road id without the lane number
 * The caller is accountable for freeing the copy
 * @param roadId Road id to get the substring from
 * @return Substring copy
 */
char* TbusSqliteDatabaseHandler::getRoadIdSubstring(const char* roadId) {
	std::string temp(roadId);

	size_t lastPosition = temp.rfind('_');
	std::string substring = temp.substr(0, lastPosition);

	char* newRoadId = new char[substring.length() + 1];
	strcpy(newRoadId, substring.c_str());

	return newRoadId;
}

/**
 * Retrieves the cell id for position (roadId, lanePos).
 * @param roadId Road id
 * @param lanePos Lane position
 * @param time Current simulation time
 * @return Cell id
 */
cellid_t TbusSqliteDatabaseHandler::getCellId(const char* const roadId, const float lanePos, simtime_t time) {
	// TODO: Get correct value
	return 1;
}

/**
 * Retrieves upload data- and droprate on edge roadId at position lanePos.
 * The parameter time is optional and defaults to the current simulation time.
 *
 * The returned object has to be destroyed by the caller.
 * @param roadId The edge to look at
 * @param lanePos The position on the edge to look at
 * @param time The time to look at (defaults to the current simulation time)
 * @return Data- and Droprate as a TbusQueueDatarateValue object
 */
TbusQueueDatarateValue* TbusSqliteDatabaseHandler::getUploadDatarate(const char* const roadId, const float lanePos, simtime_t time) {
	TbusQueueDatarateValue* result = new TbusQueueDatarateValue();

	char* newRoadId = getRoadIdSubstring(roadId);

	sqlite3_reset(uploadDatarateStatementEdge);
	sqlite3_bind_double(uploadDatarateStatementEdge, 1, lanePos);
	sqlite3_bind_text(uploadDatarateStatementEdge, 2, newRoadId, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int64(uploadDatarateStatementEdge, 3, getSimulationTimeWithOffset(time).inUnit(SIMTIME_NS));

	int sqlResult = sqlite3_step(uploadDatarateStatementEdge);

	if (sqlResult != SQLITE_ROW && sqlResult != SQLITE_DONE) {
		TBUS_NOTAROW(__FILE__, __LINE__)
		// Set dummy values
		result->datarate = TBUS_DATARATE_DEFAULT;
		result->droprate = TBUS_DROPRATE_DEFAULT;
	} else {
		if (sqlite3_column_type(uploadDatarateStatementEdge, 0) == SQLITE_NULL) {
			// Aggregation functions returns at least one row, even if it's empty
			result->datarate = TBUS_DATARATE_DEFAULT;
			result->droprate = TBUS_DROPRATE_DEFAULT;

			EV << "WARNING: Using default upload delay value!" << endl;
		} else {
			// Retrieve values from database
			result->datarate = sqlite3_column_double(uploadDatarateStatementEdge, 0) * TBUS_KBYTE_TO_BIT;
			result->droprate = sqlite3_column_double(uploadDatarateStatementEdge, 1);
		}
	}

#ifdef TBUS_VERBOSE
	EV << "Retrieved upload datarate " << result->datarate << " and droprate " << result->droprate << " for roadId " << newRoadId << " and lanePos " << lanePos << std::endl;
#endif

	delete[] newRoadId;

	return result;
}

/**
 * Retrieves upload delay on edge roadId and at position lanePos.
 * The parameter time is optional and defaults to the current simulation time.
 *
 * The returned object has to be destroyed by the caller.
 * @param roadId The edge to look at
 * @param lanePos The position on the edge to look at
 * @param time The time to look at (defaults to the current simulation time)
 * @return Delay as a TbusQueueDelayValue
 */
TbusQueueDelayValue* TbusSqliteDatabaseHandler::getUploadDelay(const char* const roadId, const float lanePos, simtime_t time) {
	TbusQueueDelayValue* result = new TbusQueueDelayValue();

	char* newRoadId = getRoadIdSubstring(roadId);

	sqlite3_reset(uploadDelayStatementEdge);
	sqlite3_bind_double(uploadDelayStatementEdge, 1, lanePos);
	sqlite3_bind_text(uploadDelayStatementEdge, 2, newRoadId, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int64(uploadDelayStatementEdge, 3, getSimulationTimeWithOffset(time).inUnit(SIMTIME_NS));

	int sqlResult = sqlite3_step(uploadDelayStatementEdge);

	if (sqlResult != SQLITE_ROW && sqlResult != SQLITE_DONE) {
		TBUS_NOTAROW(__FILE__, __LINE__)
		// Set dummy values
		result->delay = TBUS_DELAY_DEFAULT;
	} else {
		if (sqlite3_column_type(uploadDelayStatementEdge, 0) == SQLITE_NULL) {
			// Aggregation functions returns at least one row, even if it's empty
			result->delay = TBUS_DELAY_DEFAULT;

			EV << "WARNING: Using default upload delay value!" << endl;
		} else {
			// Retrieve value from database
			result->delay = sqlite3_column_int64(uploadDelayStatementEdge, 0);
		}
	}

#ifdef TBUS_VERBOSE
	EV << "Retrieved upload delay " << result->delay << " for roadId " << newRoadId << " and lanePos " << lanePos << std::endl;
#endif

	delete[] newRoadId;

	return result;
}

/**
 * Retrieves download data- and droprate on edge roadId at position lanePos.
 * The parameter time is optional and defaults to the current simulation time.
 *
 * The returned object has to be destroyed by the caller.
 * @param roadId The edge to look at
 * @param lanePos The position on the edge to look at
 * @param time The time to look at (defaults to the current simulation time)
 * @return Data- and Droprate as a TbusQueueDatarateValue object
 */
TbusQueueDatarateValue* TbusSqliteDatabaseHandler::getDownloadDatarate(const char* const roadId, const float lanePos, simtime_t time) {
	TbusQueueDatarateValue* result = new TbusQueueDatarateValue();

	char* newRoadId = getRoadIdSubstring(roadId);

	sqlite3_reset(downloadDatarateStatementEdge);
	sqlite3_bind_double(downloadDatarateStatementEdge, 1, lanePos);
	sqlite3_bind_text(downloadDatarateStatementEdge, 2, newRoadId, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int64(downloadDatarateStatementEdge, 3, getSimulationTimeWithOffset(time).inUnit(SIMTIME_NS));

	int sqlResult = sqlite3_step(downloadDatarateStatementEdge);

	if (sqlResult != SQLITE_ROW && sqlResult != SQLITE_DONE) {
		TBUS_NOTAROW(__FILE__, __LINE__)
		// Set dummy values
		result->datarate = TBUS_DATARATE_DEFAULT;
		result->droprate = TBUS_DROPRATE_DEFAULT;
	} else {
		if (sqlite3_column_type(downloadDatarateStatementEdge, 0) == SQLITE_NULL) {
			// Aggregation functions returns at least one row, even if it's empty
			result->datarate = TBUS_DATARATE_DEFAULT;
			result->droprate = TBUS_DROPRATE_DEFAULT;

			EV << "WARNING: Using default download datarate value!" << endl;
		} else {
			// Retrieve values from database
			result->datarate = sqlite3_column_double(downloadDatarateStatementEdge, 0) * TBUS_KBYTE_TO_BIT;
			result->droprate = sqlite3_column_double(downloadDatarateStatementEdge, 1);
		}
	}

#ifdef TBUS_VERBOSE
	EV << "Retrieved download datarate " << result->datarate << " and droprate " << result->droprate << " for roadId " << newRoadId << " and lanePos " << lanePos << std::endl;
#endif

	delete[] newRoadId;

	return result;
}

/**
 * Retrieves download delay on edge roadId and at position lanePos.
 * The parameter time is optional and defaults to the current simulation time.
 *
 * The returned object has to be destroyed by the caller.
 * @param roadId The edge to look at
 * @param lanePos The position on the edge to look at
 * @param time The time to look at (defaults to the current simulation time)
 * @return Delay as a TbusQueueDelayValue
 */
TbusQueueDelayValue* TbusSqliteDatabaseHandler::getDownloadDelay(const char* const roadId, const float lanePos, simtime_t time) {
	TbusQueueDelayValue* result = new TbusQueueDelayValue();

	char* newRoadId = getRoadIdSubstring(roadId);

	sqlite3_reset(downloadDelayStatementEdge);
	sqlite3_bind_double(downloadDelayStatementEdge, 1, lanePos);
	sqlite3_bind_text(downloadDelayStatementEdge, 2, newRoadId, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int64(downloadDelayStatementEdge, 3, getSimulationTimeWithOffset(time).inUnit(SIMTIME_NS));

	int sqlResult = sqlite3_step(downloadDelayStatementEdge);

	if (sqlResult != SQLITE_ROW && sqlResult != SQLITE_DONE) {
		TBUS_NOTAROW(__FILE__, __LINE__)
		// Set dummy values
		result->delay = TBUS_DELAY_DEFAULT;
	} else {

		if (sqlite3_column_type(downloadDelayStatementEdge, 0) == SQLITE_NULL) {
			// Aggregation functions returns at least one row, even if it's empty
			result->delay = TBUS_DELAY_DEFAULT;

			EV << "WARNING: Using default download delay value!" << endl;
		} else {
			// Retrieve value from database
			result->delay = sqlite3_column_int64(downloadDelayStatementEdge, 0);
		}
	}

#ifdef TBUS_VERBOSE
	EV << "Retrieved upload delay " << result->delay << " for roadId " << newRoadId << " and lanePos " << lanePos << std::endl;
#endif

	delete[] newRoadId;

	return result;
}

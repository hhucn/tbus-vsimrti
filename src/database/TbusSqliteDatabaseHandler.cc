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

	uploadCellIdStatementEdge = NULL;
	downloadCellIdStatementEdge = NULL;

	uploadGroupIdRestricted = NULL;
	uploadGroupIdUnrestricted = NULL;
	downloadGroupIdRestricted = NULL;
	downloadGroupIdUnrestricted = NULL;

	// Upload datarate
	sqlite3_prepare_v2(database, "select datarate, droprate, abs(lanePos - ?1) as dist from upload where roadId = ?2 and groupid = ?3 order by dist asc limit 1;", -1 , &uploadDatarateStatementEdge, NULL);
	// Upload delay
	sqlite3_prepare_v2(database, "select delay, abs(lanePos - ?1) as dist from upload where roadId = ?2 and groupid = ?3 order by dist asc limit 1;", -1 , &uploadDelayStatementEdge, NULL);
	// Download datarate
	sqlite3_prepare_v2(database, "select datarate, droprate, abs(lanePos - ?1) as dist from download where roadId = ?2 and groupid = ?3 order by dist asc limit 1;", -1 , &downloadDatarateStatementEdge, NULL);
	// Download delay
	sqlite3_prepare_v2(database, "select delay, abs(lanePos - ?1) as dist from download where roadId = ?2 and groupid = ?3 order by dist asc limit 1;", -1 , &downloadDelayStatementEdge, NULL);

	// Upload cell id
	sqlite3_prepare_v2(database, "select cellId, abs(lanePos - ?1) as dist from upload where roadId = ?2 and groupid = ?3 order by dist asc limit 1;", -1 , &uploadCellIdStatementEdge, NULL);
	// Download cell id
	sqlite3_prepare_v2(database, "select cellId, abs(lanePos - ?1) as dist from download where roadId = ?2 and groupid = ?3 order by dist asc limit 1;", -1 , &downloadCellIdStatementEdge, NULL);

	// Group id queries
	sqlite3_prepare_v2(database, "select groupid, (?1 - timestamp) as diff from upload where roadId = ?2 and timestamp <= ?1 order by diff asc limit 1;", -1, &uploadGroupIdRestricted, NULL);
	sqlite3_prepare_v2(database, "select groupid, abs(?1 - timestamp) as diff from upload where roadId = ?2 order by diff asc limit 1;", -1, &uploadGroupIdUnrestricted, NULL);

	sqlite3_prepare_v2(database, "select groupid, (?1 - timestamp) as diff from download where roadId = ?2 and timestamp <= ?1 order by diff asc limit 1;", -1, &downloadGroupIdRestricted, NULL);
	sqlite3_prepare_v2(database, "select groupid, abs(?1 - timestamp) as diff from download where roadId = ?2 order by diff asc limit 1;", -1, &downloadGroupIdUnrestricted, NULL);
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

	sqlite3_finalize(uploadCellIdStatementEdge);
	sqlite3_finalize(downloadCellIdStatementEdge);

	sqlite3_finalize(uploadGroupIdRestricted);
	sqlite3_finalize(uploadGroupIdUnrestricted);
	sqlite3_finalize(downloadGroupIdRestricted);
	sqlite3_finalize(downloadGroupIdUnrestricted);

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
	// Currently we're using the upload cell id value
	cellid_t result = TBUS_CELLID_DEFAULT;

	char* newRoadId = getRoadIdSubstring(roadId);
	simtime_t offsetTime = getSimulationTimeWithOffset(time);
	uint64_t groupId = getUploadGroupId(newRoadId, offsetTime);

	sqlite3_reset(uploadCellIdStatementEdge);
	sqlite3_bind_double(uploadCellIdStatementEdge, 1, lanePos);
	sqlite3_bind_text(uploadCellIdStatementEdge, 2, newRoadId, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int64(uploadCellIdStatementEdge, 3, groupId);

	int sqlResult = sqlite3_step(uploadCellIdStatementEdge);

	if (sqlResult != SQLITE_ROW && sqlResult != SQLITE_DONE) {
		TBUS_NOTAROW(__FILE__, __LINE__)
	} else {
		if (sqlite3_column_type(uploadCellIdStatementEdge, 0) == SQLITE_NULL) {
			// Aggregation functions returns at least one row, even if it's empty
			EV << "WARNING: Using default cell id value!" << endl;
		} else {
			// Retrieve values from database
			result = sqlite3_column_int64(uploadCellIdStatementEdge, 0);
		}
	}

	#ifdef TBUS_VERBOSE
		EV << "Retrieved cell id " << result << " for roadId " << newRoadId << " and lanePos " << lanePos << " and group id " << groupId << " at " << time << std::endl;
	#endif

	delete[] newRoadId;

	return result;
}

bool TbusSqliteDatabaseHandler::executeGroupIdQuery(sqlite3_stmt* query, const char* const roadId, simtime_t time) {
	sqlite3_reset(query);
	sqlite3_bind_int64(query, 1, time.inUnit(SIMTIME_NS));
	sqlite3_bind_text(query, 2, roadId, -1, SQLITE_TRANSIENT);

	int result = sqlite3_step(query);

	return (result == SQLITE_ROW);
}

uint64_t TbusSqliteDatabaseHandler::getUploadGroupId(const char* const roadId, simtime_t time) {
	uint64_t groupId = TBUS_GROUPID_DEFAULT;

	if (executeGroupIdQuery(uploadGroupIdRestricted, roadId, time)) {
		groupId = sqlite3_column_int64(uploadGroupIdRestricted, 0);
	} else if (executeGroupIdQuery(uploadGroupIdUnrestricted, roadId, time)) {
		groupId = sqlite3_column_int64(uploadGroupIdUnrestricted, 0);
	} else {
		EV << "Error retrieving upload group id, using default " << TBUS_GROUPID_DEFAULT << ": no group id found for roadId " << roadId << " at " << time << std::endl;
	}

	return groupId;
}

uint64_t TbusSqliteDatabaseHandler::getDownloadGroupId(const char* const roadId, simtime_t time) {
	uint64_t groupId = TBUS_GROUPID_DEFAULT;

	if (executeGroupIdQuery(downloadGroupIdRestricted, roadId, time)) {
		groupId = sqlite3_column_int64(downloadGroupIdRestricted, 0);
	} else if (executeGroupIdQuery(downloadGroupIdUnrestricted, roadId, time)) {
		groupId = sqlite3_column_int64(downloadGroupIdUnrestricted, 0);
	} else {
		EV << "Error retrieving download group id, using default" << TBUS_GROUPID_DEFAULT << ": no group id found for roadId " << roadId << " at " << time << std::endl;
	}

	return groupId;
}

TbusQueueDatarateValue* TbusSqliteDatabaseHandler::getDatarateValue(sqlite3_stmt* query, const char* const roadId, const float lanePos, uint64_t groupId) {
	TbusQueueDatarateValue* result = new TbusQueueDatarateValue();

	sqlite3_reset(query);
	sqlite3_bind_double(query, 1, lanePos);
	sqlite3_bind_text(query, 2, roadId, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int64(query, 3, groupId);

	int sqlResult = sqlite3_step(query);

	if (sqlResult != SQLITE_ROW && sqlResult != SQLITE_DONE) {
		TBUS_NOTAROW(__FILE__, __LINE__)
		// Set dummy values
		result->datarate = TBUS_DATARATE_DEFAULT;
		result->droprate = TBUS_DROPRATE_DEFAULT;
	} else {
		if (sqlite3_column_type(query, 0) == SQLITE_NULL) {
			// Aggregation functions returns at least one row, even if it's empty
			result->datarate = TBUS_DATARATE_DEFAULT;
			result->droprate = TBUS_DROPRATE_DEFAULT;

			EV << "WARNING: Using default data- and droprate value: " << TBUS_DATARATE_DEFAULT << " and " << TBUS_DROPRATE_DEFAULT << endl;
		} else {
			// Retrieve values from database
			result->datarate = round(sqlite3_column_double(query, 0) * TBUS_KBYTE_TO_BIT);
			result->droprate = sqlite3_column_double(query, 1);
		}
	}

#ifdef TBUS_VERBOSE
	EV << "Retrieved datarate " << result->datarate << " and droprate " << result->droprate << " for roadId " << roadId << " and lanePos " << lanePos << " and group id " << groupId << std::endl;
#endif

	return result;
}

TbusQueueDelayValue* TbusSqliteDatabaseHandler::getDelayValue(sqlite3_stmt* query, const char* const roadId, const float lanePos, uint64_t groupId) {
	TbusQueueDelayValue* result = new TbusQueueDelayValue();

	sqlite3_reset(query);
	sqlite3_bind_double(query, 1, lanePos);
	sqlite3_bind_text(query, 2, roadId, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int64(query, 3, groupId);

	int sqlResult = sqlite3_step(query);

	if (sqlResult != SQLITE_ROW && sqlResult != SQLITE_DONE) {
		TBUS_NOTAROW(__FILE__, __LINE__)
		// Set dummy values
		result->delay = TBUS_DELAY_DEFAULT;
	} else {
		if (sqlite3_column_type(query, 0) == SQLITE_NULL) {
			// Aggregation functions returns at least one row, even if it's empty
			result->delay = TBUS_DELAY_DEFAULT;

			EV << "WARNING: Using default delay value:" << TBUS_DELAY_DEFAULT << endl;
		} else {
			// Retrieve value from database
			result->delay = sqlite3_column_int64(query, 0);
		}
	}

#ifdef TBUS_VERBOSE
	EV << "Retrieved delay " << result->delay << " for roadId " << roadId << " and lanePos " << lanePos << " and group id " << groupId << std::endl;
#endif

	return result;
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
	char* newRoadId = getRoadIdSubstring(roadId);
	simtime_t offsetTime = getSimulationTimeWithOffset(time);
	uint64_t groupId = getUploadGroupId(newRoadId, offsetTime);

	TbusQueueDatarateValue* result = getDatarateValue(uploadDatarateStatementEdge, newRoadId, lanePos, groupId);

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
	char* newRoadId = getRoadIdSubstring(roadId);
	simtime_t offsetTime = getSimulationTimeWithOffset(time);
	uint64_t groupId = getUploadGroupId(newRoadId, offsetTime);

	TbusQueueDelayValue* result = getDelayValue(uploadDelayStatementEdge, newRoadId, lanePos, groupId);

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
	char* newRoadId = getRoadIdSubstring(roadId);
	simtime_t offsetTime = getSimulationTimeWithOffset(time);
	uint64_t groupId = getDownloadGroupId(newRoadId, offsetTime);

	TbusQueueDatarateValue* result = getDatarateValue(downloadDatarateStatementEdge, newRoadId, lanePos, groupId);

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
	char* newRoadId = getRoadIdSubstring(roadId);
	simtime_t offsetTime = getSimulationTimeWithOffset(time);
	uint64_t groupId = getDownloadGroupId(newRoadId, offsetTime);

	TbusQueueDelayValue* result = getDelayValue(downloadDelayStatementEdge, newRoadId, lanePos, groupId);

	delete[] newRoadId;

	return result;
}

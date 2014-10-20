/*
 * SqliteDatabaseHandler.cc
 *
 *  Created on: 13.10.2014
 *      Author: bialon
 */

#include "SqliteDatabaseHandler.h"

#define TBUS_SQLITE_DATABASE "test.sqlite"
#define TBUS_SQLITE_USER_VERSION 0x00000001

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

SqliteDatabaseHandler::~SqliteDatabaseHandler() {
	sqlite3_finalize(uploadDatarateStatement);
	sqlite3_finalize(uploadDelayStatement);
	sqlite3_finalize(downloadDatarateStatement);
	sqlite3_finalize(downloadDelayStatement);

	sqlite3_close(database);
}

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

void SqliteDatabaseHandler::abort() {
	sqlite3_close(database);
	exit(1);
}

int SqliteDatabaseHandler::uploadCallbackHandler(SqliteDatabaseHandler* handler, int numCols, char** colValues, char** colNames) {
	return handler ? handler->uploadCallback(numCols, colValues, colNames) : -1;
}

int SqliteDatabaseHandler::uploadCallback(int numCols, char** colValues, char** colNames) {
	return 0;
}

int SqliteDatabaseHandler::downloadCallbackHandler(SqliteDatabaseHandler* handler, int numCols, char** colValues, char** colNames) {
	return handler ? handler->downloadCallback(numCols, colValues, colNames) : -1;
}

int SqliteDatabaseHandler::downloadCallback(int numCols, char** colValues, char** colNames) {
	return 0;
}

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
		std::cerr << __FILE__ << " (" << __LINE__ << "): Unable to retrieve values from database - query result is not a row!" << std::endl;
		// Set dummy values
		result->datarate = 0.0;
		result->droprate = 0.0;
	} else {
		// Retrieve values from database
		result->datarate = sqlite3_column_double(uploadDatarateStatement, 0) * 1024 * 1024; // MBit/s -> Bit/s
		result->droprate = sqlite3_column_double(uploadDatarateStatement, 1);
	}

	return result;
}

TbusQueueDatarateValue* SqliteDatabaseHandler::getUploadDatarate(const Coord& pos) {
	return this->getUploadDatarate(pos, simTime());
}

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
		std::cerr << __FILE__ << " (" << __LINE__ << "): Unable to retrieve values from database - query result is not a row!" << std::endl;
		// Set dummy values
		result->delay = 0.0;
	} else {
		// Retrieve values from database
		result->delay = sqlite3_column_double(uploadDelayStatement, 0) * 10e-9; // nsec -> sec
	}

	return result;
}

TbusQueueDelayValue* SqliteDatabaseHandler::getUploadDelay(const Coord& pos) {
	return this->getUploadDelay(pos, simTime());
}

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
		std::cerr << __FILE__ << " (" << __LINE__ << "): Unable to retrieve values from database - query result is not a row!" << std::endl;
		// Set dummy values
		result->datarate = 0.0;
		result->droprate = 0.0;
	} else {
		// Retrieve values from database
		result->datarate = sqlite3_column_double(downloadDatarateStatement, 0);
		result->droprate = sqlite3_column_double(downloadDatarateStatement, 1);
	}

	return result;
}

TbusQueueDatarateValue* SqliteDatabaseHandler::getDownloadDatarate(const Coord& pos) {
	return this->getDownloadDatarate(pos, simTime());
}

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
		std::cerr << __FILE__ << " (" << __LINE__ << "): Unable to retrieve values from database - query result is not a row!" << std::endl;
		// Set dummy values
		result->delay = 0.0;
	} else {
		// Retrieve values from database
		result->delay = sqlite3_column_double(downloadDelayStatement, 0);
	}

	return result;
}

TbusQueueDelayValue* SqliteDatabaseHandler::getDownloadDelay(const Coord& pos) {
	return this->getDownloadDelay(pos, simTime());
}

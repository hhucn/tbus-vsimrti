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
		sqlite3_close(database);
		exit(1);
	}

	// Check databases' user_version for compatibility
	int32_t version = getDatabaseVersion();
	if (version != TBUS_SQLITE_USER_VERSION) {
		std::cerr << "Expected database version " << TBUS_SQLITE_USER_VERSION << ", but found version "<< version << "! Exiting..." << std::endl;
		sqlite3_close(database);
		exit(1);
	}
}

SqliteDatabaseHandler::~SqliteDatabaseHandler() {
	sqlite3_close(database);
}

int32_t SqliteDatabaseHandler::getDatabaseVersion() {
	sqlite3_stmt* statement;

	sqlite3_prepare_v2(database, "pragma user_version;", -1, &statement, NULL);
	if (sqlite3_step(statement) != SQLITE_ROW) {
		std::cerr << "Unable to retrieve database version! Exiting..." << std::endl;
		sqlite3_close(database);
		exit(1);
	}

	int32_t version = sqlite3_column_int(statement, 0);
	sqlite3_finalize(statement);

	return version;
}

int SqliteDatabaseHandler::callbackHandler(SqliteDatabaseHandler* handler, int numCols, char** colValues, char** colNames) {
	return handler ? handler->callback(numCols, colValues, colNames) : -1;
}

int SqliteDatabaseHandler::callback(int numCols, char** colValues, char** colNames) {
	return 0;
}

TbusQueueDatarateValue* SqliteDatabaseHandler::getDatarate(Coord& pos, simtime_t time) {
	return NULL;
}

TbusQueueDatarateValue* SqliteDatabaseHandler::getDatarate(Coord& pos) {
	return NULL;
}

TbusQueueDelayValue* SqliteDatabaseHandler::getDelay(Coord& pos, simtime_t time) {
	return NULL;
}

TbusQueueDelayValue* SqliteDatabaseHandler::getDelay(Coord& pos) {
	return NULL;
}

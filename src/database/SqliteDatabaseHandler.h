/*
 * SqliteDatabaseHandler.h
 *
 *  Created on: 13.10.2014
 *      Author: bialon
 */

#ifndef SQLITEDATABASEHANDLER_H_
#define SQLITEDATABASEHANDLER_H_

#include "DatabaseHandler.h"

class SqliteDatabaseHandler : public DatabaseHandler {
	public:
		SqliteDatabaseHandler();
		virtual ~SqliteDatabaseHandler();
};

#endif /* SQLITEDATABASEHANDLER_H_ */

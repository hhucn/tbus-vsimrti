/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef LOGCDF_H_
#define LOGCDF_H_

#include <omnetpp.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>

namespace projekt {

class LogCDF {

	private:

		// static constants for parsing
		static const int MAX_CHARS_PER_LINE = 64;

		// saving all parsed values in vector
		std::vector<long int> values;
		std::vector<long int> count;

		// given file path
		std::string headerComment;
		std::string filePath;

		virtual int readFile();
		virtual void sortValues();
		virtual void writeFile();

	public:
		LogCDF();
		virtual ~LogCDF();

		bool sortFile(std::string path);
};

} /* namespace projekt */
#endif /* LOGCDF_H_ */

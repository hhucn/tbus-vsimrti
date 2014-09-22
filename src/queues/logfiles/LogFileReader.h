/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef LOGFILEREADER_H_
#define LOGFILEREADER_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include <omnetpp.h>
#include <Preamble.h>

#include <LogDatarate.h>
#include <LogPackets.h>
#include <LogBbdelay.h>

namespace projekt {

	class LogFileReader {
		private:
			// static constants for parsing
			static const int MAX_CHARS_PER_LINE = 512;
			static const int MAX_TOKENS_PER_LINE = 20;

			const char* kTRUE_LOWERCASE;

		public:
			// constructor / destructor
			LogFileReader();
			virtual ~LogFileReader();

			// parsing functions
			std::vector<LogDatarate*> parseDatarate(std::string path, std::string delimiter, std::vector<LogDatarate*> logData, long limit);
			std::vector<LogBbdelay*> parseBbdelay(std::string path, std::string delimiter, std::vector<LogBbdelay*> logBbdelay, long limit);
			std::vector<LogPackets*> parsePackets(std::string path, std::string delimiter, std::vector<LogPackets*> logPackets, long limit);
	};

} /* namespace projekt */
#endif /* LOGFILEREADER_H_ */

/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "LogCDF.h"

namespace projekt {

LogCDF::LogCDF() {
}

LogCDF::~LogCDF() {
}

/**
 * Reads file, like in LogFileReader
 * @return 0, when everything is fine
 */
int LogCDF::readFile() {
	// create a file-reading object
	std::ifstream fin;

	// open a file
	fin.open(filePath.c_str());
	std::cout << "good: " << fin.good() << "\n";

	// exit if file not found
	if (!fin.good()) {
		std::cout << filePath << " is not good()\n";
		return 1;
	}

	// delimiter between entries
	std::string delimiter = "";

	// read each line of the file
	while (!fin.eof()) {
		// init buffer
		char buf[MAX_CHARS_PER_LINE];
		fin.getline(buf, MAX_CHARS_PER_LINE);
//		const char* token = buf;
		// get token
//		token = strtok(buf, delimiter.c_str());

		// store token
		if ((strstr(buf, "#") - buf) == 0) {
			headerComment = buf;
		} else {
			values.push_back(atol(buf));
		}
	}
	return 0;
}

/**
 * Using build-in std::sort
 */
void LogCDF::sortValues() {
	std::sort(values.begin(), values.end());
	int c = 1;
	for (unsigned int i=0; i<values.size()-1; i++){
		while (((i+1)<values.size()) && (values.at(i) == values.at(i+1))){
			c++;
			values.erase(values.begin()+i);
		}
		count.push_back(c);
	}
}

/**
 * writes file, like the LogFileWriter
 */
void LogCDF::writeFile() {
	// collect all counts
	long maxCount = 0;
	double cdf = 0;
	for (unsigned int i=0; i<count.size(); i++){
		maxCount += count.at(i);
	}

	// open file in output and append mode
	std::ofstream outputFile;
	outputFile.open(filePath.c_str(), std::ios::out | std::ios::app);

	// when file is open
	if (outputFile.is_open()) {
		// write comment
		outputFile << headerComment << "\n";
		// and all sorted values / cdf
		for (unsigned int i = 0; i < values.size(); i++) {
			cdf += ((double)count.at(i)) / ((double) maxCount);
			outputFile << values.at(i) << "," << cdf << "\n";
		}
	} else {
		std::cout << "LogCDF could not open " << filePath << "\n";
	}

	outputFile.flush();
	outputFile.close();
}

/**
 * Will sort all entries in given file. Format should be:
 * #comment <br>
 * long <br>
 * long <br>
 * ...
 * @param path
 * @return
 */
bool LogCDF::sortFile(std::string path) {
	std::cout << "Start manipulating " << path << "\n";

	// reset values
	filePath = path;
	values.clear();
	headerComment = "";

	if (readFile() == 0) {
		sortValues();
		writeFile();
		std::cout << "Sorted values in " << path << ".\n\n";
	} else {
		std::cout << "Error while reading " << path << ".\n\n";
		return false;
	}

	remove(path.c_str());

	return true;
}

} /* namespace projekt */

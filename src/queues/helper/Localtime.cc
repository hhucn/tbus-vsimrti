/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "Localtime.h"

namespace projekt {

/**
 * default constrctor
 */
Localtime::Localtime() {
}

/**
 * default destrcutor
 */
Localtime::~Localtime() {
}


/**
 * Get current system time in milliseconds
 * @return current system time in ms
 */
std::string Localtime::currentTimeInMilliseconds() {
	char buff[40];
	time_t timer;
	struct tm y2k;
	double seconds;

	// init current time struct
	y2k.tm_hour = 0;
	y2k.tm_min = 0;
	y2k.tm_sec = 0;
	y2k.tm_year = 100;
	y2k.tm_mon = 0;
	y2k.tm_mday = 1;

	time(&timer); /* get current time; same as: timer = time(NULL)  */

	seconds = difftime(timer, mktime(&y2k));

	printf("%.f seconds since January 1, 2000 in the current timezone", seconds);
	sprintf(buff, "%.f", seconds);
	return std::string(buff);
}

/**
 * Get current date, format is YYYY-MM-DD
 * @param withSeperator, true, when return value will be %Y-%m-%d, else without minus/spaces
 * @return current date as string with/less seperator
 */
std::string Localtime::currentDate(bool withSeperator) {
	char buff[40];
	time_t now = time(NULL);
	if (withSeperator) strftime(buff, 20, "%Y-%m-%d", localtime(&now));
	else strftime(buff, 20, "%Y%m%d", localtime(&now));
	return std::string(buff);
}

/**
 * Get current date/time
 * @param withSeperator, true, when return value will be YYYY-MM-DD.HH:mm, else without minus/spaces
 * @return current date and time as string with/less seperator
 */
std::string Localtime::currentDateTimeShort(bool withSeperator) {
	char buff[40];
	time_t now = time(NULL);
	if (withSeperator) strftime(buff, 20, "%Y-%m-%d %H:%M", localtime(&now));
	else strftime(buff, 20, "%Y%m%d_%H%M", localtime(&now));
	return std::string(buff);
}

/**
 * Get current date/time
 * @param withSeperator, true, when return value will be %Y-%m-%d_%H:%M:%S, else without minus/spaces
 * @return current date and time as string with/less seperator
 */
std::string Localtime::currentDateTimeLong(bool withSeperator) {
	char buff[40];
	time_t now = time(NULL);
	if (withSeperator) strftime(buff, 20, "%Y-%m-%d_%H:%M:%S", localtime(&now));
	else strftime(buff, 20, "%Y%m%d_%H%M%S", localtime(&now));
	return std::string(buff);
}

/**
 * Get current dateand time with underscore
 * @param withSeperator, true, when return value will be %Y-%m-%d_%H-%M, else without minus/spaces
 * @return current date and time as string with/less seperator
 */
std::string Localtime::currentLocaltimeForLogfile(bool withSeperator) {
	char buff[40];
	time_t now = time(NULL);
	if (withSeperator) strftime(buff, 20, "%Y-%m-%d_%H-%M", localtime(&now));
	else strftime(buff, 20, "%Y%m%d_%H%M", localtime(&now));
	return std::string(buff);
}
}

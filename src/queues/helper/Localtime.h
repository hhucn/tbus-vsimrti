/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

namespace projekt {
	class Localtime {
		public:
			Localtime();
			virtual ~Localtime();

			virtual std::string currentTimeInMilliseconds();
			virtual std::string currentDate(bool withSeperator);
			virtual std::string currentDateTimeShort(bool withSeperator);
			virtual std::string currentDateTimeLong(bool withSeperator);
			virtual std::string currentLocaltimeForLogfile(bool withSeperator);
	};
}

#endif /* LOCALTIME_H_ */

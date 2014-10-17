//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "TbusCoordinateConverter.h"
#include "omnetpp.h"
#include <sstream>

TbusCoordinateConverter::TbusCoordinateConverter() {
	init("+proj=latlong +ellps=wgs84");
}

TbusCoordinateConverter::TbusCoordinateConverter(const char* initString) {
	init(initString);
}

void TbusCoordinateConverter::init(const char* initString) {
	projLatlong  = pj_init_plus(initString);
	projMercator = pj_init_plus("+ proj=merc +ellps=wgs84");

	if (!projLatlong || !projMercator) {
		std::ostringstream ostream;
		ostream << "Failed to initialize PROJ.4 with initialization string \"" << initString << "\"!";
		opp_error(ostream.str().c_str());
	}
}

TbusCoordinateConverter::~TbusCoordinateConverter() {
	// TODO Auto-generated destructor stub
}

Coord TbusCoordinateConverter::translate(const Coord* pos) const {
	double x = pos->x * DEG_TO_RAD;
	double y = pos->y * DEG_TO_RAD;

	int result = pj_transform(projLatlong, projMercator, 1, 1, &x, &y, NULL);

	if (!result) {
		std::ostringstream ostream;
		ostream << "Translating coordinates failed with error code " << result << "!";
		opp_error(ostream.str().c_str());
	}

	EV << "translated (" << pos->x << "," << pos->y << ") to (" << x << "," << y << ")!" << std::endl;
	std::cout << "translated (" << pos->x << "," << pos->y << ") to (" << x << "," << y << ")!" << std::endl;

	return Coord(x,y);
}

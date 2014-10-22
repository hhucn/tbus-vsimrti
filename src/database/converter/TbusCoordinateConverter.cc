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

// This has to be the same string and offset SUMO uses!
Register_GlobalConfigOption(CFGID_TBUS_LIBPROJ_INIT_STRING, "tbus-libproj-init", CFG_STRING, "+proj=utm +zone=33 +ellps=WGS84 +datum=WGS84 +units=m +no_defs", "The PROJ.4 initialization string TBUS shall use for input coordinates. Output is always latlong");
Register_GlobalConfigOption(CFGID_TBUS_SUMO_OFFSET_X, "tbus-sumo-offset-x", CFG_DOUBLE, "0.0", "SUMOs x offset (so that pos.x + offset.x = coord.x)");
Register_GlobalConfigOption(CFGID_TBUS_SUMO_OFFSET_Y, "tbus-sumo-offset-y", CFG_DOUBLE, "0.0", "SUMOs y offset (so that pos.y + offset.y = coord.y)");

/**
 * Instantiate with values read from *.ini-file or default values.
 */
TbusCoordinateConverter::TbusCoordinateConverter() :
	offset_x(ev.getConfig()->getAsDouble(CFGID_TBUS_SUMO_OFFSET_X)),
	offset_y(ev.getConfig()->getAsDouble(CFGID_TBUS_SUMO_OFFSET_Y)) {
	// Init with config init string
	init(ev.getConfig()->getAsString(CFGID_TBUS_LIBPROJ_INIT_STRING).c_str());
}

/**
 * Instantiates with offset values read from *.ini-file or defaul values and projection string initString
 * @param initString Projection string to use for libproj
 */
TbusCoordinateConverter::TbusCoordinateConverter(const char* initString) :
		offset_x(ev.getConfig()->getAsDouble(CFGID_TBUS_SUMO_OFFSET_X)),
		offset_y(ev.getConfig()->getAsDouble(CFGID_TBUS_SUMO_OFFSET_Y)) {
	init(initString);
}

/**
 * Singleton access.
 * Returns the singleton instance of this class with memory automatically handled.
 * @return Singleton instance
 */
TbusCoordinateConverter* TbusCoordinateConverter::getInstance() {
	static TbusCoordinateConverter instance;

	return &instance;
}

/**
 * Initializes projections with latitude/longitude projection and projection defined by initString.
 * Aborts on libproj failing to initialize.
 * @param initString Projection string to use for libproj
 */
void TbusCoordinateConverter::init(const char* initString) {
	projLatlong  = pj_init_plus("+proj=latlong");
	projMercator = pj_init_plus(initString);

	if (!projLatlong) {
		opp_error("Failed to initialize PROJ.4 with latlong projection!");
	}
	if (!projMercator) {
		std::ostringstream ostream;
		ostream << "Failed to initialize PROJ.4 with initialization string \"" << initString << "\"!";
		opp_error(ostream.str().c_str());
	}
}

/**
 * Empty destructor.
 */
TbusCoordinateConverter::~TbusCoordinateConverter() {}

/**
 * Translates coordinates from projection defined by initString to latitude/longitude values.
 * Utilizes libproj's pj_transform.
 * Considers offset if given (default offset is (0,0)).
 * @param pos Coordinates to translate
 * @return Translated coordinates in degrees (NOT radiant)
 */
Coord TbusCoordinateConverter::translate(const Coord* pos) const {
	double x = pos->x + offset_x;
	double y = pos->y + offset_y;

	// UTM -> Latitude/Longitude
	int result = pj_transform(projMercator, projLatlong, 1, 1, &x, &y, NULL);

	if (result != 0) {
		std::ostringstream ostream;
		ostream << "Translating coordinates failed with error code " << result << ": " << pj_strerrno(result) << "!";
		opp_error(ostream.str().c_str());
	}

	// Radiant -> Degrees
	x = x * RAD_TO_DEG;
	y = y * RAD_TO_DEG;

	// Swap x and y because of Latitude, Longitude
	return Coord(y,x);
}

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

#ifndef TBUSCOORDINATECONVERTER_H_
#define TBUSCOORDINATECONVERTER_H_

#include <proj_api.h>
#include "Coord.h"

class TbusCoordinateConverter {
	private:
		projPJ projMercator;
		projPJ projLatlong;

		inline void init(const char* initString);

	public:
		TbusCoordinateConverter();
		TbusCoordinateConverter(const char* initString);
		virtual ~TbusCoordinateConverter();

		Coord translate(const Coord* pos) const;
};

#endif /* TBUSCOORDINATECONVERTER_H_ */

//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
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

#ifndef TBUSQUEUECONTROL_H_
#define TBUSQUEUECONTROL_H_

#include <csimplemodule.h>

#include "CDRQ.h"
#include "CRRQ.h"
#include "CRSQ.h"
#include "CDSQ.h"
#include "Coord.h"

class TbusQueueControl : public cSimpleModule {
	private:
		projekt::CDRQ* cdrq;
		projekt::CRRQ* crrq;
		projekt::CRSQ* crsq;
		projekt::CDSQ* cdsq;

		int cdrqControl;
		int crrqControl;
		int crsqControl;
		int cdsqControl;

	public:
		TbusQueueControl();
		virtual ~TbusQueueControl();

		void updateQueues(const Coord& newCoords);

		void initialize();
};

#endif /* TBUSQUEUECONTROL_H_ */

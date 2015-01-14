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

#ifndef TBUSQUEUEVALUE_H_
#define TBUSQUEUEVALUE_H_

#include "omnetpp.h"

/**
 * Abstract base class for queue values.
 */
class TbusQueueValue : public cObject {
	public:
		simtime_t time; ///< Simulation time of object creation

		/**
		 * Initializes time with the current simulation time.
		 */
		TbusQueueValue() : time(simTime()) {};
		/**
		 * Empty destructor.
		 */
		virtual ~TbusQueueValue() {};

		/**
		 * Pure virtual method for comparison
		 * @param other Object to compare against
		 * @return Comparison value
		 */
		virtual inline bool operator!=(TbusQueueValue& other) = 0;
};

#endif /* TBUSQUEUEVALUE_H_ */

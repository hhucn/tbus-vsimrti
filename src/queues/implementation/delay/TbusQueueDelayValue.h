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

#ifndef TBUSQUEUEDELAYVALUE_H_
#define TBUSQUEUEDELAYVALUE_H_

#include "TbusQueueValue.h"

/**
 * Delay queue value.
 * Stores delay and also offers a comparison operator.
 */
class TbusQueueDelayValue : public TbusQueueValue {
	public:
		simtime_t delay; ///< Delay value.

		TbusQueueDelayValue();
		TbusQueueDelayValue(const TbusQueueDelayValue& other);

		virtual bool operator!=(TbusQueueValue& other);
};

#endif /* TBUSQUEUEDELAYVALUE_H_ */

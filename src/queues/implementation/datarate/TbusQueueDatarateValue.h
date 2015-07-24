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

#ifndef TBUSQUEUEDATARATEVALUE_H_
#define TBUSQUEUEDATARATEVALUE_H_

#include "TbusQueueValue.h"

/**
 * Datarate queue value.
 * Stores data- and droprate and also offers a comparison operator.
 */
class TbusQueueDatarateValue : public TbusQueueValue {
	public:
		TbusQueueDatarateValue();
		TbusQueueDatarateValue(const TbusQueueDatarateValue& other);

		bool operator!=(TbusQueueValue& other);

		bool isValid();

		double droprate; ///< Droprate in percent
		int64_t datarate; ///< Datarate in Bit/s
};

#endif /* TBUSQUEUEDATARATEVALUE_H_ */

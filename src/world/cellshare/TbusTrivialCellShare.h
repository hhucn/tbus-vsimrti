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

#ifndef TBUSTRIVIALCELLSHARE_H_
#define TBUSTRIVIALCELLSHARE_H_

#include "TbusCellShare.h"
#include <map>
#include <set>

/**
 * A trivial CellShare model for TBUS.
 * @author Raphael Bialon <raphael.bialon@hhu.de>
 */
class TbusTrivialCellShare : public TbusCellShare {
	public:
		TbusTrivialCellShare();

		virtual TbusQueueDatarateValue* adaptDatarateValue(cellid_t cellId, TbusQueueDatarateValue* value, TbusHost* host = NULL);
		virtual TbusQueueDelayValue* adaptDelayValue(cellid_t cellId, TbusQueueDelayValue* value, TbusHost* host = NULL);
};

#endif /* TBUSTRIVIALCELLSHARE_H_ */

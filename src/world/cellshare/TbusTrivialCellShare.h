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

/**
 * A trivial CellShare model for TBUS.
 * @author Raphael Bialon <raphael.bialon@hhu.de>
 */
class TbusTrivialCellShare : public TbusCellShare {
	private:
		/**
		 * Maps a cell id to the number of connected hosts.
		 */
		std::map<cellid_t, int> cellToNumHosts;

	public:
		TbusTrivialCellShare();
		virtual ~TbusTrivialCellShare();

		virtual void hostMoved(cellid_t from, cellid_t to, cModule* host = NULL);

		virtual void adaptValue(cellid_t cellId, TbusQueueValue* value, cModule* host = NULL);
};

#endif /* TBUSTRIVIALCELLSHARE_H_ */

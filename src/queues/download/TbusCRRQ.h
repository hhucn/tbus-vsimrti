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

#ifndef TBUSCRRQ_H_
#define TBUSCRRQ_H_

#include "TbusDatarateQueue.h"

/**
 * Client dataRate Receive Queue.
 * This queue behaves just as #TbusDatarateQueue, so no changes have been made other than deriving.
 * @see TbusDatarateQueue
 */
class TbusCRRQ : public TbusDatarateQueue {
	public:
		TbusCRRQ();
		virtual void initialize();
};

#endif /* TBUSCRRQ_H_ */

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

#ifndef TBUSDELAYQUEUETESTER_H_
#define TBUSDELAYQUEUETESTER_H_

#include "TbusTestBase.h"
#include "TbusDelayQueue.h"

/**
 * Test class for #TbusDelayQueue.
 */
class TbusDelayQueueTester : public TbusTestBase<TbusDelayQueueTester> {
	private:
			int inGate; ///< Input gate id
			int outGate; ///< Output gate id

			TbusDelayQueue* queue; ///< Queue to test

	public:
		TbusDelayQueueTester();
		virtual ~TbusDelayQueueTester();

		void initialize();

		void testConstantDelay();
		void handleConstantDelay(cMessage* msg);

		void testNoDelay();
		void handleNoDelay(cMessage* msg);

		void testChangingDelay();
		void handleChangingDelay(cMessage* msg);
};

#endif /* TBUSDELAYQUEUETESTER_H_ */

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

#ifndef TBUSDATARATEQUEUETESTER_H_
#define TBUSDATARATEQUEUETESTER_H_

#include <csimplemodule.h>
#include "TbusTestBase.h"
#include "TbusDatarateQueue.h"
#include "omnetpp.h"

class TbusDatarateQueueTester : public cSimpleModule, public TbusTestBase<TbusDatarateQueueTester> {
	private:
		int inGate;
		int outGate;

		simtime_t expectedTime;
		TbusDatarateQueue* datarateQueue;

	public:
		TbusDatarateQueueTester();
		virtual ~TbusDatarateQueueTester();

		void initialize();
		void handleMessage(cMessage* msg);

		void testNormalDatarate();
};

#endif /* TBUSDATARATEQUEUETESTER_H_ */

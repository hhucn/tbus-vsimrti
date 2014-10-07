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

package tbus.queues.implementation.test.ned;

import tbus.queues.implementation.test.ned.TbusDatarateQueueTest;
import tbus.queues.implementation.test.ned.TbusDatarateQueueTester;

network TbusTestNetwork
{
    parameters:

        @display("bgb=327,182");
    submodules:
        datarateQueue: TbusDatarateQueueTest {
            @display("p=54,50");
        }
        datarateTester: TbusDatarateQueueTester {
            @display("p=271,50");
        }
        delayQueue: TbusDelayQueueTest {
            @display("p=54,128");
        }
        delayTester: TbusDelayQueueTester {
            @display("p=271,128");
        }

    connections:
        datarateTester.outGate --> datarateQueue.inGate;
        datarateTester.inGate <-- datarateQueue.outGate;

        delayTester.outGate --> delayQueue.inGate;
        delayTester.inGate <-- delayQueue.outGate;
}

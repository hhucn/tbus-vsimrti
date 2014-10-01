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

package tbus.queues2.test.ned;

import tbus.queues2.test.ned.TbusDatarateQueueTest;
import tbus.queues2.test.ned.TbusDatarateQueueTester;

network TbusTestNetwork
{
    parameters:

        @display("bgb=327,119");
    submodules:
        testQueue: TbusDatarateQueueTest {
            @display("p=42,50");
        }
        tester: TbusDatarateQueueTester {
            @display("p=291,50");
        }

    connections:
        tester.outGate --> testQueue.inGate;
        tester.inGate <-- testQueue.outGate;
}

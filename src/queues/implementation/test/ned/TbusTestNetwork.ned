//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
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

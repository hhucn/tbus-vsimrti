//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
// 
package tbus.queues.implementation.test.ned;

simple TbusDelayQueueTest {
    parameters:
        @class(TbusDelayQueue);
	gates:
	    input inGate;
	    output outGate;
}

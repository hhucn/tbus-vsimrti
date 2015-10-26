//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
// 

package tbus.ned;

simple TbusCDRQ {
    parameters:
        int queueLength = default(-1); // TBUS queue length, -1 disables queue buffer and makes it infinite
	
	gates:
	    input inGate;
	    output outGate;
}
//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//
//
// Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
//

package tbus.queues.ned;

// Client Delay Receive Queue
// Queue for simulating download backbone delay. Contains in and outgate, as well as extra gate for control message
// Signals are included, but will not be collected
simple CDRQ {
    // all parameters
	parameters:
		@display("i=block/queue");    // displaying as queue

        //# all signals with type
		@signal[CDRQ_bbdelay]	(type="simtime_t");    // signal for emitting bbdelay as double
		
        //# alternativ: collecting stats
        // will display signal 'CDRQ_bbdelay' as histogram and vector, with no interpolation
		@statistic[bbdelay]		(title="CDRQ bb delay";	source="CDRQ_bbdelay"; unit="simtime_t"; record=histogram,vector; interpolationmode=none);
        
    // all in, out and control gates
	gates:
		input in;             // input gate, connected to Router
		output out;           // output gate, connected to CRRQ
		inout controlPort;    // control gate, connected to main object, here a node
}

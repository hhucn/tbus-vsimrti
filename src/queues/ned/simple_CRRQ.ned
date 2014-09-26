//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//
//
// Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
//

package tbus.queues.ned;

// Client Datarate Receive Queue
// Queue for simulating upload datarate delay. Contains in and outgate, as well as extra gate for control message
// Signals are included, but will not be collected
simple CRRQ {
    // all parameters
    parameters:
        @display("i=block/subqueue");    // displaying as subqueue
		
		//# all signals with type
		@signal[CRRQ_datarate]				(type="double");    // signal for emitting datarate as double
		@signal[CRRQ_lossprob]				(type="double");    // signal for emitting lossprobability as double
        @signal[CRRQ_lossprob_rnd]          (type="double");    // signal for emitting random for lossing a packet double
		@signal[CRRQ_packet_datarate_delay]	(type="simtime_t"); // signal for emitting packet delay as simtime_t
		@signal[CRRQ_drop_measured]         (type="int");       // signal for emitting measured packet drop
        @signal[CRRQ_drop_simulated]        (type="int");       // signal for emitting simulated packet drop
        

		//# alternativ: collecting stats
        // will display signal 'CRRQ_datarate' as histogram and vector, with no interpolation
		@statistic[datarate](title="CRRQ datarate";	source="CRRQ_datarate";	unit="double"; record=histogram,vector; interpolationmode=linear);
        // will display signal 'CRRQ_lossprob' as vector, with no interpolation
		@statistic[lossprob](title="CRRQ loss probability"; source="CRRQ_lossprob"; unit="double"; record=vector; interpolationmode=sample-hold);
        // will display signal 'CRRQ_lossprob_rnd' as vector, with no interpolation
        @statistic[lossprobRnd](title="CRRQ loss probability rnd"; source="CRRQ_lossprob_rnd"; unit="double"; record=vector; interpolationmode=none);
        // will display signal 'CRRQ_packet_datarate_delay' as histogram and vector, with no interpolation
		@statistic[packetdataratedelay](title="CRRQ bb delay"; source="CRRQ_packet_datarate_delay"; unit="simtime_t"; record=histogram,vector; interpolationmode=none);
		// will display signal 'CRRQ_drop_measured' as vector, with no interpolation
        @statistic[dropmeasured](title="CRRQ drop measured"; source="CRRQ_drop_measured"; record=vector; unit="int"; interpolationmode=none);
        // will display signal 'CRRQ_drop_simulated' as and vector, with no interpolation
        @statistic[dropsimulated](title="CRRQ drop simulated"; source="CRRQ_drop_simulated"; record=vector; unit="int"; interpolationmode=none);
		
    // all in, out and control gates
    gates:
        input in;             // input gate, connected to CDRQ
        output out;           // output gate, connected to Node
        inout controlPort;    // control gate, connected to main object, here a node
}

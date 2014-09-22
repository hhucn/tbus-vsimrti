//
//@author Tobias Krauthoff
//@email tobias.krauthoff@hhu.de
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//

package tbus.queues.ned;

// Client Datarate Send Queue
// Queue for simulating download datarate delay. Contains in and outgate, as well as extra gate for control message
// Signals are included, but will not be collected
simple CRSQ
{
    // all parameters
	parameters:
		@display("i=block/queue");    // displaying as queue
		
        //# all signals with type
		@signal[CRSQ_datarate]				(type="double");    // signal for emitting datarate as double
		@signal[CRSQ_lossprob]				(type="double");    // signal for emitting lossprobability as double
        @signal[CRSQ_lossprob_rnd]          (type="double");    // signal for emitting random for lossing a packet double
		@signal[CRSQ_packet_datarate_delay]	(type="simtime_t"); // signal for emitting packet delay as simtime_t
        @signal[CRSQ_drop_measured]         (type="int");       // signal for emitting measured packet drop
        @signal[CRSQ_drop_simulated]        (type="int");       // signal for emitting simulated packet drop


		//# alternativ: collecting stats
        // will display signal 'CRSQ_datarate' as histogram and vector, with no interpolation
		@statistic[datarate](title="CRSQ datarate";	source="CRSQ_datarate";	record=histogram,vector; unit="double"; interpolationmode=linear);
        // will display signal 'CRSQ_lossprob' as vector, with no interpolation
		@statistic[lossprob](title="CRSQ loss probability"; source="CRSQ_lossprob"; record=vector; unit="double";  interpolationmode=sample-hold);
        // will display signal 'CRSQ_lossprob_rnd' as vector, with no interpolation
        @statistic[lossprobRnd](title="CRSQ loss probability rnd"; source="CRSQ_lossprob_rnd"; record=vector; unit="double";  interpolationmode=none);
        // will display signal 'CRSQ_packet_datarate_delay' as histogram and vector, with no interpolation
		@statistic[packetdataratedelay](title="CRSQ bb delay"; source="CRSQ_packet_datarate_delay";	record=histogram,vector; unit="double"; interpolationmode=none);
		// will display signal 'CRSQ_drop_measured' as vector, with no interpolation
        @statistic[dropmeasured](title="CRSQ drop measured"; source="CRSQ_drop_measured"; record=vector; unit="int"; interpolationmode=none);
        // will display signal 'CRSQ_drop_simulated' as and vector, with no interpolation
        @statistic[dropsimulated](title="CRSQ drop simulated"; source="CRSQ_drop_simulated"; record=vector; unit="int"; interpolationmode=none);
        
		
        //# attributes
        bool writingLogFileNode             = default(false);   // when true, logfiles for behaviour of node should be collected
        bool writingLogFileQueue            = default(false);   // true, when logfiles for behaviour of CRSQ should be collected
        string outputPathPrefix				= default("");      // string for user-specific path for logfiles
        
    // all in, out and control gates
	gates:
        input in;             // input gate, connected to Node
        output out;           // output gate, connected to CDSQ
        inout controlPort;    // control gate, connected to main object, here a node
}

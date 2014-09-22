//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//

package tbus.queues.ned;

// MobileNode, which will connected to upload and dowload queues for simulating datarate and backbone delays.
// A few simtimes will be logged and a lot of options are included
simple MobileNode
{
    // all parameters
    parameters:
        @signal[mobile_node_traveldelay]	(type="simtime_t");    // signal for emiting travel delay as simtime_t
        @signal[mobile_node_arrivaltime]	(type="simtime_t");    // signal for emiting arrival time as simtime_t
        @signal[mobile_node_sendingtime]	(type="simtime_t");    // signal for emiting sending time as simtime_t
        @signal[mobile_node_trainlength]	(type="int");          // signal for train length delay as int
        @signal[mobile_node_drop_measured]  (type="int");          // signal for emitting measured packet drop
        @signal[mobile_node_drop_simulated] (type="int");          // signal for emitting simulated packet drop
        
        //# interpolationmode values are none, sample-hold, backward-sample-hold, linear.
		//# alternativ: collecting stats
		//
        @statistic[delay] (title="delay time"; source="mobile_node_traveldelay"; record=histogram,vector; unit="simtime_t"; interpolationmode=none);
        // will display signal 'mobile_node_traveldelay' as vector, with no interpolation
        @statistic[arrivalTime] (title="arrival time"; source="mobile_node_arrivaltime"; record=vector; unit="simtime_t"; interpolationmode=none);
        // will display signal 'mobile_node_arrivaltime' as vector, with no interpolation
        @statistic[sendingTime] (title="sending time"; source="mobile_node_sendingtime"; record=vector; unit="simtime_t"; interpolationmode=none);
        // will display signal 'mobile_node_sendingtime' as vector, with no interpolation
        @statistic[trainLength] (title="train length"; source="mobile_node_trainlength"; record=vector; unit="int"; interpolationmode=none);
        // will display signal 'mobile_node_trainlength' as vector, with no interpolation
        @statistic[dropmeasured](title="mobile node drop measured"; source="mobile_node_drop_measured"; record=vector; unit="int"; interpolationmode=none);
        // will display signal 'mobile_node_drop_simulated' as and vector, with no interpolation
        @statistic[dropsimulated](title="mobile node drop simulated"; source="mobile_node_drop_simulated"; record=vector; unit="int"; interpolationmode=none);

        //# attributes
        @display("i=device/pc2");
        bool sendInitialMessage				= default(false);  // true will be enabled, when sendLogFileMessages is false; will send random message trains for testing
        bool sendLogFileMessages			= default(false);  // when true, will send message input from ../../simulations/loggingrun1010/*
        bool showMsgBox						= default(false);  // when true, a message box will show some information at startup; CURRENT WINDOWS ONLY
        bool changePacketsDelayForTesting	= default(false);  // when true and sendInitialMessage is true, random delays will be created and send to all queues
        bool writingLogFileNode 			= default(false);  // when true, logfiles for behaviour of node should be collected
        int maximalTrainLength				= default(32767);  // maximal length per train
        int simTimeLimit					= default(3600);   // simulation time length in seconds
        int trainIntervall					= default(1000);   // intervall for sending trains in milliseconds
        string ip							= default("127.0.1.0");  // string for custrom ip
        string port                         = default("1234");  // string for custrom port
        string outputPathPrefix				= default("");   // string for user-specific path for logfiles
        string inputPathPrefix              = default("");  // string for user-specific path for input

    // all gates
    gates:
        input in;               // input gate
        output out;             // output gate
		inout controlPortCRSQ;  // control gate (new delays) for CRSQ
		inout controlPortCDSQ;  // control gate (new delays) for CDSQ
		inout controlPortCDRQ;  // control gate (new delays) for CDRQ
		inout controlPortCRRQ;  // control gate (new delays) for CRRQ
}

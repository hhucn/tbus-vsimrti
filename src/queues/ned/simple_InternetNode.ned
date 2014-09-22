//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//

package tbus.queues.ned;

// Simple Node in Internet, no queues, no delays
// A few simtimes will be logged and a lot of options are included
simple InternetNode
{
    // all parameters
    parameters:
        @signal[inet_node_traveldelay] (type="simtime_t");         // signal for emiting travel delay as simtime_t
        @signal[inet_node_drop_measured]  (type="int");          // signal for emitting measured packet drop
        @signal[inet_node_drop_simulated] (type="int");          // signal for emitting simulated packet drop
        
        //# interpolationmode values are none, sample-hold, backward-sample-hold, linear.
        // will display signal 'inet_node_traveldelay' as vector, with linear interpolation
        @statistic[delay] (title="delay time"; source="inet_node_traveldelay"; record=histogram,vector; unit="simtime_t"; interpolationmode=linear);
        // will display signal 'inet_node_drop_measured' as vector, with no interpolation
        @statistic[dropmeasured](title="internet node drop measured"; source="inet_node_drop_measured"; record=vector; unit="int"; interpolationmode=none);
        // will display signal 'inet_node_drop_simulated' as and vector, with no interpolation
        @statistic[dropsimulated](title="internet node drop simulated"; source="inet_node_drop_simulated"; record=vector; unit="int"; interpolationmode=none);

        // attributes
        @display("i=device/pc2");
        bool sendInitialMessage     = default(false);         // true will be enabled, when sendLogFileMessages is false; will send random message trains for testing
        bool sendLogFileMessages    = default(false);         // when true, will send message input from ../../simulations/loggingrun1010/*
        bool writingLogFileNode     = default(false);         // when true, logfiles for behaviour of node should be collected
        bool showMsgBox				= default(false);         // when true, a message box will show some informations at startup; CURRENT WINDOWS ONLY
        string ip					= default("127.0.1.0");   // string for custrom ip
        string port                 = default("1234");        // string for custrom port
        string outputPathPrefix		= default("");            // string for user-specific path for logfiles
        string inputPathPrefix      = default("");            // string for user-specific path for input

    gates:
        input in;   // simple input gate
        output out; // simple output gate
}

//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//

package tbus.queues.ned;

// Simple Queue ned with seperated in and out gate
simple Queue
{
    // all parameters
    parameters:
        @display("bgb=,,white;i=block/queue"); // queue will be displayed as queue ;)

    // all parameters
    gates:
        input in;   // just input for queue
        output out; // and output for queue

}
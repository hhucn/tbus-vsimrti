//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//

package tbus.queues.ned;

// Router with many ports and a queue for every port
module Router
{
    // all parameters
    parameters:
        string ip = default("127.0.0.0");           // default ip
        //# not needed anymore:
        //# int clientCount = default(1);               // client count, needed for creating ports
        @display("i=device/router;bgb=,,white");    // style attributes

    // all parameters
    gates:
        inout gate[]; // input/output gate array, for every connected module

    // all parameters
    submodules:
        // module is wrappter for simple_router
        routing: Routing {
            @display("p=40,35");
        }
        // queues for every output gate
        queue[sizeof(gate)]: Queue {
            @display("p=140,35,c,60");

        }
        
    // all connections, we allow unconnected
    connections allowunconnected:
        for i=0..sizeof(gate)-1 {                                   // for every gate
            gate$i[i] -->  routing.gate$i++;      // connect gate in with router in
            routing.gate$o++ -->  queue[i].in;    // connect router outgate with queue ingate
            queue[i].out -->  gate$o[i];          // connect queue outgate with gate outgate
        }
}





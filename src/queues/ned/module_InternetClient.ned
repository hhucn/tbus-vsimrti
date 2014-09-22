//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//

package tbus.queues.ned;

// Interclient, which contains the InternetNode, nothing else
module InternetClient
{
    // all parameters
    parameters:
        string ip;  // custom string for ip in network
        @display("bgb=,,white;i=device/smallrouter");   // display as router

    // all gates
    gates:
        input in;   // input gate from router
        output out; // output gate to router

    // all submodules
    submodules:
        // Module is wrapper for simple_InternetNode
        InternetNode: InternetNode {
            @display("p=69,73;i=block/ifcard;is=l");
        }

    // all connections, we allow unconnected
    connections allowunconnected:
        in --> InternetNode.in;
        InternetNode.out --> out;
}


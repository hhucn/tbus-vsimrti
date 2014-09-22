//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//

package tbus.queues.ned;

// Simple Router .ned with inout gates
simple Routing
{
    // all parameters
    parameters:
        @display("bgb=,,white;i=device/server");

    // all parameters
    gates:
        inout gate[];// @loose;

}
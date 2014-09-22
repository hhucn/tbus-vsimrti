//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//

package tbus.queues.ned;

// MobileClient, just the module which contains and connects all four queues with a MobileNode
module MobileClient
{
    // all parameters
    parameters:
        string ip;  // string for ip from simple_MobileNode
        @display("bgb=,,white;i=device/cpu");

    // all gates
    gates:
        input in;   // simple input gate
        output out; // simple output gate

    // all submodules
    submodules:
        // Client Data Rate Send Queue
        CRSQ: CRSQ {
            @display("p=40,180");
        }
        // Client Data Rate Received Queue
        CRRQ: CRRQ {
            @display("p=300,180;i=block/queue");
        }
        // Client Delay Send Queue
        CDSQ: CDSQ {
            @display("p=80,80;i=block/subqueue");
        }
        // Client Delay Received Queue
        CDRQ: CDRQ {
            @display("p=260,80;i=block/subqueue");
        }
        // Mobile Node
        MobileNode: MobileNode {
            @display("p=170,180;i=block/ifcard;is=l");
        }

    // all connections, we allow unconnected
    connections allowunconnected:
        in --> CDRQ.in;
        CDRQ.out --> CRRQ.in;
        CRRQ.out -->  MobileNode.in;
        MobileNode.out -->  CRSQ.in;
        CRSQ.out -->  CDSQ.in;
        CDSQ.out --> out;

        MobileNode.controlPortCRSQ <-->  CRSQ.controlPort;
        MobileNode.controlPortCRRQ <-->  CRRQ.controlPort;
        MobileNode.controlPortCDSQ <-->  CDSQ.controlPort;
        MobileNode.controlPortCDRQ <-->  CDRQ.controlPort;
}


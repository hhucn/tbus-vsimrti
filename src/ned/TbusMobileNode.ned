//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
// 
package tbus.ned;

import tbus.ned.TbusMobileRadio;

import inet.base.NotificationBoard;
import inet.transport.udp.UDP;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.nodes.inet.NetworkLayer;

import federate.mob.VSimRTIMobility;
import federate.msg.VSimRTIUnreliableApp;

module TbusMobileNode
{
    parameters:
        @class(TbusMobileNode);
        @node();
        @display("bgb=351,209");

    gates:
        input radioIn @directIn;

    submodules:
        notificationBoard: NotificationBoard {
            @display("p=268,32");
        }
        urapp: VSimRTIUnreliableApp {
            @display("p=47,164");
            maxProcDelay = 0.0;
        }
        udp: UDP {
            @display("p=133,164");
        }
        interfaceTable: InterfaceTable {
            @display("p=86,32");
        }
        networkLayer: NetworkLayer {
            parameters:
                // Enable proxyARP and globalARP
                // because TBUS includes ARP delay in its delay values
                // and the connection: mobile device <-> base station
                // is merely a point-to-point-connection without the need for ARP
                proxyARP = true;
                arp.globalARP = true;
                ip.forceBroadcast = false;
                ip.procDelay = 0s;
                @display("p=219,164");
            gates:
                ifIn[1];
                ifOut[1];
        }
        routingTable: RoutingTable {
            @display("p=86,90");
        }
        radio: TbusMobileRadio {
            @display("p=307,164");
        }
        mobility: VSimRTIMobility {
            @display("p=268,90");
        }
    connections allowunconnected:
        // App <-> Transport
        urapp.udpOut --> udp.appIn++;
        urapp.udpIn <-- udp.appOut++;
        // Transport <-> Network
        udp.ipOut --> networkLayer.udpIn;
        udp.ipIn <-- networkLayer.udpOut;
        // Network <-> MAC and PHY
        networkLayer.ifOut[0] --> radio.upperLayerIn;
        networkLayer.ifIn[0] <-- radio.upperLayerOut;
		// PHY <- Channel
        radio.radioIn <-- radioIn;
}

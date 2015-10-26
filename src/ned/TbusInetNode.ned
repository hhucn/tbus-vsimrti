//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
// 
package tbus.ned;

import tbus.ned.TbusInetRadio;
import inet.base.NotificationBoard;
import inet.transport.udp.UDP;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.nodes.inet.NetworkLayer;

import federate.mob.VSimRTIMobility;
import federate.msg.VSimRTIUnreliableApp;

module TbusInetNode
{
    parameters:
        @node();
        @class(TbusInetNode);
        @display("i=device/router;bgb=265,380");

    gates:
        input radioIn @direct;

    submodules:
        notificationBoard: NotificationBoard {
            @display("p=70,217");
        }
        interfaceTable: InterfaceTable {
            @display("p=70,147");
        }
        routingTable: RoutingTable {
            parameters:
                IPForward = true;
                routerId = "";
                routingFile = "";
                @display("p=70,86");
        }
        networkLayer: NetworkLayer {
            parameters:
                proxyARP = false;
                arp.globalARP = true;
                ip.forceBroadcast = false;
                ip.procDelay = 0s;
                @display("p=182,150");
            gates:
                ifIn[1];
                ifOut[1];
        }
        udp: UDP {
            @display("p=182,220");
        }
        urapp: VSimRTIUnreliableApp {
            parameters:
            	@display("p=182,287");
            	maxProcDelay = 0.0;
        }
        radio: TbusInetRadio;
        mobility: VSimRTIMobility {
            @display("p=70,284");
        }

    connections allowunconnected:
        networkLayer.udpIn <-- udp.ipOut;
        networkLayer.udpOut --> udp.ipIn;

        udp.appIn++ <-- urapp.udpOut;
        udp.appOut++ --> urapp.udpIn;

        // Network <-> MAC and PHY
        networkLayer.ifOut[0] --> radio.upperLayerIn;
        networkLayer.ifIn[0] <-- radio.upperLayerOut;
		// PHY <- Channel
        radio.radioIn <-- radioIn;
}

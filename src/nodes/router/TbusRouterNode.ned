//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package tbus.nodes.router;

//import tbus.applications.router.TbusRoutingApplication;
//import tbus.networklayer.router.TbusRouter;

import inet.base.NotificationBoard;
import inet.transport.udp.UDP;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.nodes.inet.NetworkLayer;

import federate.msg.VSimRTIUnreliableApp;

module TbusRouterNode
{
    parameters:
        @node();
        @class("TbusRouterNode");
        @display("i=device/router;bgb=468,380");

    gates:
        input channelInGate;
        output channelOutGate;
        input inetInGate @loose;
        output inetOutGate @loose;

    submodules:
//        routingApp: TbusRoutingApplication {
//            @display("p=382,28");
//        }
//        router: TbusRouter {
//            @display("p=319,113");
//        }
        notificationBoard: NotificationBoard {
            @display("p=58,220");
        }
        interfaceTable: InterfaceTable {
            @display("p=58,150");
        }
        routingTable: RoutingTable {
            parameters:
                IPForward = true;
                routerId = "";
                routingFile = "";
                @display("p=58,89");
        }
        networkLayer: NetworkLayer {
            parameters:
                proxyARP = false;
                @display("p=296,171");
            gates:
                ifIn[1];
                ifOut[1];
        }
        udp: UDP {
            @display("p=296,244");
        }
        //        app: TbusMobileApplication;
        urapp: VSimRTIUnreliableApp {

            @display("p=296,326");
        }

    connections allowunconnected:
//        routingApp.lowerOutGate --> router.upperInGate;
//        routingApp.lowerInGate <-- router.upperOutGate;

        //	    router.channelInGate <-- channelInGate;
        //	    router.channelOutGate --> channelOutGate;
        channelOutGate <-- networkLayer.ifOut[0];
        channelInGate --> networkLayer.ifIn[0];

        networkLayer.udpIn <-- udp.ipOut;
        networkLayer.udpOut --> udp.ipIn;

        udp.appIn++ <-- urapp.udpOut;
        udp.appOut++ --> urapp.udpIn;

//        router.inetInGate <-- inetInGate;
//        router.inetOutGate --> inetOutGate;
}

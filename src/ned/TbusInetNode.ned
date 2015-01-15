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
            @display("p=182,287");
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

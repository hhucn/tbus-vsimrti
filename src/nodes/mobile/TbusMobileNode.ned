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

package tbus.nodes.mobile;

import tbus.linklayer.radio.TbusRadio;

import inet.base.NotificationBoard;
import inet.transport.udp.UDP;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.nodes.inet.NetworkLayer;

import federate.mob.VSimRTIMobility;
import federate.msg.VSimRTIUnreliableApp;

module TbusMobileNode {
    parameters:
        @node();
        @display("bgb=351,348");

    gates:
        input radioIn @directIn;

    submodules:
        notificationBoard: NotificationBoard {
            @display("p=58,220");
        }
        urapp: VSimRTIUnreliableApp {
            @display("p=295,291");
            maxProcDelay = 0;
        }
        udp: UDP {
            @display("p=295,209");
        }
        interfaceTable: InterfaceTable {
            @display("p=58,150");
        }
        networkLayer: NetworkLayer {
            parameters:
                proxyARP = true;
                arp.globalARP = false;
                ip.forceBroadcast = false;
                ip.procDelay = 0s;
                @display("p=295,136");
            gates:
                ifIn[1];
                ifOut[1];
        }
        routingTable: RoutingTable {
                @display("p=58,89");
        }
        radio: TbusRadio {
            @display("p=295,65");
        }
        mobility: VSimRTIMobility {
            @display("p=58,32");
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

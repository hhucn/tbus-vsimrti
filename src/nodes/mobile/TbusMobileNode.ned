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
import tbus.applications.mobile.TbusMobileApplication;

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
        @display("bgb=741,506");
        @class("TbusMobileNode");

    gates:
        input radioIn @loose;

    submodules:
        radio: TbusRadio {
            @display("p=295,65");
        }
        mobility: VSimRTIMobility {
            @display("p=58,32");
        }
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
            	@display("p=295,136");
            gates:
                ifIn[1];
                ifOut[1];
        }
        udp: UDP {
            @display("p=295,209");
        }
        //        app: TbusMobileApplication;
        urapp: VSimRTIUnreliableApp {

            @display("p=295,291");
        }
    connections allowunconnected:
        //        radio.upperInGate  <-- app.lowerOutGate;
        //        radio.upperOutGate --> app.lowerInGate;
        
        // App <-> Transport
        urapp.udpOut --> udp.appIn++;
        urapp.udpIn <-- udp.appOut++;
        // Transport <-> Network
        udp.ipOut --> networkLayer.udpIn;
        udp.ipIn <-- networkLayer.udpOut;
        // Network <-> PHY (We skip MAC here)
        networkLayer.ifOut[0] --> radio.upperInGate;
        networkLayer.ifIn[0] <-- radio.upperOutGate;
		// PHY <- Channel
        radio.radioIn <-- radioIn;
        
//        TODO: UDP-Pakete richtig durchreichen
}

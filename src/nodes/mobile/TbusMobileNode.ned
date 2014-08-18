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

import federate.mob.VSimRTIMobility;

module TbusMobileNode
{
    parameters:
        @display("bgb=429,267");
        
    gates:
        input radioIn @loose;
        
    submodules:
        radio: TbusRadio {
            @display("p=363,189");
        }
        mobility: VSimRTIMobility {
            @display("p=31,29");
        }
        notificationBoard: NotificationBoard {
            @display("p=77,228");
        }
        app: TbusMobileApplication;

    connections:
        radio.upperInGate  <-- app.lowerOutGate;
        radio.upperOutGate --> app.lowerInGate;
        radio.radioIn <-- radioIn;
}

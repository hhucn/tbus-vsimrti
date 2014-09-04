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

package tbus.linklayer.radio;

import tbus.linklayer.radio.TbusRadioPHY;
import tbus.linklayer.radio.TbusRadioMAC;

module TbusRadio
{
    parameters:
        @display("i=block/ifcard");
    gates:
        input upperLayerIn;
        output upperLayerOut;
        input radioIn @directIn;
    submodules:
        phy: TbusRadioPHY {
            @display("p=50,145");
        }
        mac: TbusRadioMAC {
            @display("p=50,58");
        }
    connections:
        upperLayerIn --> mac.upperLayerIn;
        upperLayerOut <-- mac.upperLayerOut;
        mac.lowerLayerIn <-- phy.upperOutGate;
        mac.lowerLayerOut --> phy.upperInGate;
        radioIn --> phy.radioIn;
}

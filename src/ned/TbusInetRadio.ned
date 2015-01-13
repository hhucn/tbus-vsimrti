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

import tbus.ned.TbusInetMAC;
import tbus.ned.TbusInetPHY;

module TbusInetRadio
{
    parameters:
        @display("i=block/ifcard;bgb=99,193");

    gates:
        input upperLayerIn;
        output upperLayerOut;
        input radioIn @directIn;

    submodules:
        mac: TbusInetMAC;
        phy: TbusInetPHY {
            @display("p=50,123");
        }

    connections:
        radioIn --> phy.radioIn;

        phy.upperLayerOut --> mac.lowerLayerIn;
        phy.upperLayerIn <-- mac.lowerLayerOut;

        mac.upperLayerOut --> upperLayerOut;
        mac.upperLayerIn <-- upperLayerIn;
}

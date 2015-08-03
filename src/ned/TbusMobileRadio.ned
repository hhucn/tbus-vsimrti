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

import tbus.ned.TbusMobilePHY;
import tbus.ned.TbusMobileMAC;
import tbus.ned.TbusQueueControl;
import tbus.ned.TbusCDRQ;
import tbus.ned.TbusCDSQ;
import tbus.ned.TbusCRRQ;
import tbus.ned.TbusCRSQ;

module TbusMobileRadio
{
    parameters:
        @display("i=block/ifcard");

    gates:
        input upperLayerIn;
        output upperLayerOut;
        input radioIn @directIn;

    submodules:
        phy: TbusMobilePHY {
            @display("p=285,155;t=phy,,black");
        }
        mac: TbusMobileMAC {
            @display("p=284,64");
        }
        queueControl: TbusQueueControl {
            @display("p=506,64");
        }
        cdrq: TbusCDRQ {
            @display("p=173,155");
        }
        cdsq: TbusCDSQ {
            @display("p=390,155");
        }
        crrq: TbusCRRQ {
            @display("p=75,155");
        }
        crsq: TbusCRSQ {
            @display("p=506,155");
        }

    connections:
        upperLayerIn --> mac.upperLayerIn;
        upperLayerOut <-- mac.upperLayerOut;

        mac.lowerLayerIn <-- crrq.outGate;
        mac.lowerLayerOut --> crsq.inGate;

        crrq.inGate <-- cdrq.outGate;
        crsq.outGate --> cdsq.inGate;

        cdrq.inGate <-- phy.upperLayerOut;
        cdsq.outGate --> phy.upperLayerIn;

        radioIn --> phy.radioIn;
}

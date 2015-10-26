//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
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

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

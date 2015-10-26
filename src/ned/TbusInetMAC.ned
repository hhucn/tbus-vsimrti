package tbus.ned;

simple TbusInetMAC {
    parameters:
        @class(TbusInetMAC);

	gates:
	    input upperLayerIn;
	    output upperLayerOut;
	    input lowerLayerIn;
	    output lowerLayerOut;
}

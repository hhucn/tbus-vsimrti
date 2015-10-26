package tbus.ned;

simple TbusInetPHY {
    parameters:
        @class(TbusInetPHY);
        
	gates:
	    input  upperLayerIn;
	    output upperLayerOut;
	    input radioIn @directIn;
}

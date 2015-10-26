//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
// 
package tbus.ned;

simple TbusMobilePHY {
    parameters:
        @class("TbusMobilePHY");
        
	gates:
	    input  upperLayerIn;
	    output upperLayerOut;
	    input radioIn @directIn;
}

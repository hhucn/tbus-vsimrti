//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
// 
package tbus.ned;

simple TbusMobileMAC {
    parameters:
        @class("TbusMobileMAC");
        
	gates:
	    input upperLayerIn;
	    output upperLayerOut;
	    input lowerLayerIn;
	    output lowerLayerOut;
}

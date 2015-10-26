package tbus.ned;

import inet.world.ChannelControlExtended;

simple TbusChannelControl extends ChannelControlExtended
{
    parameters:
    	@class(TbusChannelControl);
    	@display("i=misc/cloud");
    	@labels(node);
    gates:
        input routerInGate @loose;
        output routerOutGate @loose;
}

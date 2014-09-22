//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//

package tbus.queues.ned;

// Ideal Channel with no delays and error rates
channel IdealChannel extends ned.DatarateChannel
{
    // additional parameters
    parameters:
		@display("ls=black,1,s;tt=message");  // display settings, here black, solid with tooltip 'message*
		int cost = default(0);                // cost for channel are equals zero
		delay = 0ns;                          // we have no delay
		per = 0;                              // no packer error rate
		ber = 0;                              // no bit error rate
//		datarate = 1000*1000Gbps;
}

//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
// 
package tbus.ned;

simple TbusQueueControl {
	parameters:
		@display("");
		
		int cdrq_defaultDelay = default(100000000); // Default CDRQ delay in ns, default 0.1s
		int cdsq_defaultDelay = default(100000000); // Default CDSQ delay in ns, default 0.1s
		
		int crrq_defaultDatarate = default(81920); // Default CRRQ datarate in Bit/s, default 10KByte/s
		double crrq_defaultLossProbability = default(0.0); // Default CRRQ loss probability, default 0%
		
		int crsq_defaultDatarate = default(81920); // Default CRSQ datarate in Bit/s, default 10KByte/s
		double crsq_defaultLossProbability = default(0.0); // Default CRSQ loss probability, default 0%
}

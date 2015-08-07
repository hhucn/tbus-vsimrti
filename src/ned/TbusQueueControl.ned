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

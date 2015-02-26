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

#ifndef TBUSMOBILEMAC_H_
#define TBUSMOBILEMAC_H_

#include "InterfaceEntry.h"
#include "MACAddress.h"
#include "omnetpp.h"

/**
 * MAC layer of TBUS NIC.
 */
class TbusMobileMAC : public cSimpleModule {
	private:
		InterfaceEntry* interfaceEntry; ///< Interface entry of NIC
		MACAddress macAddress; ///< MAC address

		int upperLayerIn, upperLayerOut; ///< Upper layer gate ids
		int lowerLayerIn, lowerLayerOut; ///< Lower layer gate ids

	public:
		TbusMobileMAC();
		virtual ~TbusMobileMAC();

		void initialize(int stage);
		/**
		 * Number of needed initialization stages, this class needs stage 0 and 2
		 * @return 3
		 */
		int numInitStages() const { return 3; }

		void handleMessage(cMessage* msg);
};

#endif /* TBUSMOBILEMAC_H_ */

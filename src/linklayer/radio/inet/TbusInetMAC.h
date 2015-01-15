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

#ifndef TBUSINETMAC_H_
#define TBUSINETMAC_H_

#include "csimplemodule.h"
#include "NotificationBoard.h"
#include "InterfaceEntry.h"
#include "MACAddress.h"

class TbusInetMAC : public cSimpleModule {
	public:
		TbusInetMAC();
		virtual ~TbusInetMAC();

		void initialize(int stage);
		/**
		 * Number of needed initialization stages, this class needs stage 0 and 2
		 * @return 3
		 */
		int numInitStages() const { return 3; }

		void handleMessage(cMessage* msg);

	private:
		NotificationBoard* nb; ///< Notification board reference
		InterfaceEntry* interfaceEntry; ///< Interface entry of NIC
		MACAddress macAddress; ///< MAC address

		int upperLayerIn; ///< Upper layer input gate
		int upperLayerOut; ///< Upper layer output gate
		int lowerLayerIn; ///< Lower layer input gate
		int lowerLayerOut; ///< Lower layer output gate
};

#endif /* TBUSINETMAC_H_ */

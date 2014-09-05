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

#ifndef TBUSRADIOMAC_H_
#define TBUSRADIOMAC_H_

#include "InterfaceEntry.h"
#include "MACAddress.h"
#include "omnetpp.h"
#include "INotifiable.h"
#include "NotificationBoard.h"

class TbusRadioMAC : public cSimpleModule, public INotifiable {
	public:
		TbusRadioMAC();
		virtual ~TbusRadioMAC();

		void receiveChangeNotification(int category, const cObject* details);

		void initialize(int stage);

		void handleMessage(cMessage* msg);

	private:
		NotificationBoard* nb;
		InterfaceEntry* interfaceEntry;
		MACAddress macAddress;
};

#endif /* TBUSRADIOMAC_H_ */

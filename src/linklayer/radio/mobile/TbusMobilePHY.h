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

#ifndef TBUSMOBILEPHY_H_
#define TBUSMOBILEPHY_H_

#include "ChannelAccess.h"
#include "Coord.h"
#include "omnetpp.h"
#include "TbusQueueControl.h"

/**
 * PHY layer of TBUS NIC.
 */
class TbusMobilePHY : public ChannelAccess {
private:
	int upperLayerIn; ///< Upper layer input gate id
	int upperLayerOut; ///< Upper layer output gate id
	int radioIn; ///< Input from link layer

	TbusChannelControl* tbusCC; ///< Channel control reference
	TbusQueueControl* queueControl; ///< Queue control reference

protected:
	void handleUpperMessage(cMessage* msg);
	void handleLowerMessage(cMessage* msg);

	void sendToChannel(cMessage* msg);

public:
    TbusMobilePHY();
    virtual ~TbusMobilePHY();

    virtual void initialize(int stage);
    virtual int numInitStages() const { return 3; }
    virtual void handleMessage(cMessage* msg);

    virtual void receiveChangeNotification(int category, const cObject *details);
};

#endif /* TBUSMOBILEPHY_H_ */

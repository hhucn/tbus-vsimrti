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

#ifndef TBUSCHANNELCONTROL_H_
#define TBUSCHANNELCONTROL_H_

#include "ChannelControl.h"
#include "TbusAirFrame_m.h"
#include "TbusMessage_m.h"

class TbusChannelControl : public ChannelControl {
private:
	int routerInGate;
	int routerOutGate;
public:
    TbusChannelControl();
    virtual ~TbusChannelControl();

    void initialize(int stage);
    void handleMessage(cMessage* msg);

//    void sendToChannel(cSimpleModule *srcRadioMod, HostRef srcHost, AirFrame *airFrame);

    void sendToRouter(TbusAirFrame* airFrame);
    void sendToClient(TbusMessage* message, HostRef Client);
    void sendToChannel(cMessage* msg);

    void updateHostPosition(HostRef h, const Coord& pos);
};

#endif /* TBUSCHANNELCONTROL_H_ */

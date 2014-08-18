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

#ifndef TBUSRADIO_H_
#define TBUSRADIO_H_

#include "ChannelAccess.h"
#include "TbusAirFrame_m.h"
#include "Coord.h"

class TbusRadio : public ChannelAccess {
private:
	int uppergateIn;
	int uppergateOut;
	int radioIn;

	const Coord getHostCoord();

protected:
	void handleSelfMessage(cMessage* msg);
	void handleUpperMessage(cMessage* msg);
	void handleLowerMessage(cMessage* msg);

	void sendToChannel(TbusAirFrame* msg);

public:
    TbusRadio();
    virtual ~TbusRadio();

    void initialize(int stage);
    void handleMessage(cMessage* msg);
    void finish();

    void receiveChangeNotification(int category, const cPolymorphic *details);
};

#endif /* TBUSRADIO_H_ */

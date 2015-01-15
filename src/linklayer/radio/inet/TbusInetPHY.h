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

#ifndef TBUSINETPHY_H_
#define TBUSINETPHY_H_

#include "ChannelAccess.h"
#include "TbusChannelControl.h"

class TbusInetPHY : public ChannelAccess {
	private:
		int upperLayerIn; ///< Upper layer input gate id
		int upperLayerOut; ///< Upper layer output gate id
		int radioIn; ///< Input from link layer

		TbusChannelControl* tbusCC; ///< Channel control reference

	protected:
		void handleUpperMessage(cMessage* msg);
		void handleLowerMessage(cMessage* msg);

		void sendToChannel(cMessage* msg);

	public:
		TbusInetPHY();
		virtual ~TbusInetPHY();

		virtual void initialize(int stage);
		/**
		 * Number of initialization stages. This class needs stage 0 and 3.
		 * @return 3
		 */
		virtual int numInitStages() const { return 4; }
		virtual void handleMessage(cMessage* msg);
};

#endif /* TBUSINETPHY_H_ */

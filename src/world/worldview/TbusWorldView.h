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

#ifndef TBUSWORLDVIEW_H_
#define TBUSWORLDVIEW_H_

#include "TbusQueueControl.h"

#include <set>

class TbusWorldView {
	private:
		TbusWorldView();
		TbusWorldView(const TbusWorldView&);
		void operator=(const TbusWorldView&);

		typedef std::set<TbusQueueControl*> QueueControlSet;
		QueueControlSet queueControls; ///< Set of queue controls
		uint64_t currentUpdated; ///< Number of currently updated queue controls

		void performUpdateRound();

	public:
		static TbusWorldView* getInstance();

		void registerQueueControl(TbusQueueControl* qc);
		void unregisterQueueControl(TbusQueueControl* qc);

		void nodeMoved(TbusQueueControl* qc, const char* const roadId, const float lanePos);

		virtual ~TbusWorldView();
};

#endif /* TBUSWORLDVIEW_H_ */

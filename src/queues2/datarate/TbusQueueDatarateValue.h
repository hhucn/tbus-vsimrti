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

#ifndef TBUSQUEUEDATARATEVALUE_H_
#define TBUSQUEUEDATARATEVALUE_H_

#include <base/TbusQueueValue.h>

class TbusQueueDatarateValue : public TbusQueueValue {
	public:
		TbusQueueDatarateValue();
		virtual ~TbusQueueDatarateValue();

		virtual void initialize();

		virtual bool operator!=(TbusQueueValue& other);

		// Droprate in percent
		double droprate;
		// Datarate in bytes per nanosecond
		double datarate;
};

#endif /* TBUSQUEUEDATARATEVALUE_H_ */
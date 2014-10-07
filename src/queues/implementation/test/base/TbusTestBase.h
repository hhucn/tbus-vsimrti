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

#ifndef TBUSTESTBASE_H_
#define TBUSTESTBASE_H_

#include <queue>
#include "csimplemodule.h"
#include "cmessage.h"
#include "TbusBaseQueue.h"

#define NEXT_TEST_MESSAGE "next.test.message"

template <class T> class TbusTestBase : public cSimpleModule {
	private:
		typedef void (T::*testFunctionPtr)(void);
		typedef void (T::*handleFunctionPtr)(cMessage*);
		typedef struct {
				testFunctionPtr test;
				handleFunctionPtr handle;
				std::string description;
		} Test;
		std::queue< Test* > tests;
		T* instance;
		handleFunctionPtr currentHandle;

	protected:
		bool success;
		cMessage nextTestMessage;

		TbusTestBase(T* i);

		void addTest(testFunctionPtr test, handleFunctionPtr handle, std::string description);
		void runNextTest();

	public:
		virtual ~TbusTestBase();

		void handleMessage(cMessage* msg);
};

#endif /* TBUSTESTBASE_H_ */

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

/**
 * Self message name for next test self message.
 */
#define NEXT_TEST_MESSAGE "next.test.message"

/**
 * Base class for queue tests.
 */
template <class T> class TbusTestBase : public cSimpleModule {
	private:
		typedef void (T::*testFunctionPtr)(void); ///< Pointer to test function
		typedef void (T::*handleFunctionPtr)(cMessage*); ///< pointer to message handling function
		/**
		 * Test struct.
		 * Contains pointers to test and message handling function and description text.
		 */
		typedef struct {
				testFunctionPtr test; ///< Function pointer to test function
				handleFunctionPtr handle; ///< Function pointer to message handler
				std::string description; ///< Test description
		} Test;
		std::queue< Test* > tests; ///< Tests queue
		T* instance; ///< Stored instance to run tests upon
		handleFunctionPtr currentHandle; ///< Current message handling function pointer

	protected:
		bool success; ///< Current test success
		cMessage nextTestMessage; ///< Self message for next test

		TbusTestBase(T* i);

		void addTest(testFunctionPtr test, handleFunctionPtr handle, std::string description);
		void runNextTest();

	public:
		virtual ~TbusTestBase();

		void handleMessage(cMessage* msg);
};

#endif /* TBUSTESTBASE_H_ */

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

#include "TbusTestBase.h"
#include "TbusDatarateQueueTester.h"
#include "TbusDelayQueueTester.h"

/**
 * Stores test instance, initializes self message and invalidates current message handler.
 * @param i Test instance to use
 */
template <class T> TbusTestBase<T>::TbusTestBase(T* i) :
	instance(i),
	currentHandle(NULL),
	nextTestMessage(NEXT_TEST_MESSAGE) {}

/**
 * Empty destructor (Cleanup is done in derived classes)
 */
template <class T> TbusTestBase<T>::~TbusTestBase() {}

/**
 * Message handler.
 * Extracts self messages and acts accordingly (starts the next test in line).
 * Also outputs the test result of the current test
 * @param msg Message to handle
 */
template <class T> void TbusTestBase<T>::handleMessage(cMessage* msg) {
	if (msg->isSelfMessage() && (strcmp(msg->getName(), NEXT_TEST_MESSAGE) == 0)) {
		EV << "Test " << tests.front()->description;
		std::cout << "Test " << tests.front()->description;
		if (success) {
			EV << " succeeded!";
			std::cout << " succeeded!";
		} else {
			EV << " failed!";
			std::cout << " failed!";
		}
		EV << std::endl;
		std::cout << std::endl;

		tests.pop();
		runNextTest();
	} else if (currentHandle) {
		(instance->*currentHandle)(msg);
	}
}

/**
 * Add a test with test function test, message handling function handle and description to the queue.
 * @param test Test function
 * @param handle Message handler
 * @param description Test description
 */
template <class T> void TbusTestBase<T>::addTest(testFunctionPtr test, handleFunctionPtr handle, std::string description) {
	Test* newTest = new Test();
	newTest->test = test;
	newTest->handle = handle;
	newTest->description = description;
	tests.push(newTest);
}

/**
 * Runs the next test.
 * Sets current test and message handle, resets test success.
 * If no more tests are available, a finish message is outputted.
 */
template <class T> void TbusTestBase<T>::runNextTest() {
	if (!tests.empty()) {
		Test* currentTest = tests.front();
		testFunctionPtr test = currentTest->test;
		currentHandle = currentTest->handle;

		success = true;

		(instance->*test)();
	} else {
		EV << "Tests finished." << std::endl;
		std::cout << "Tests finished." << std::endl;
	}
}

// Template instantiation for linker
template class TbusTestBase<TbusDatarateQueueTester>;
template class TbusTestBase<TbusDelayQueueTester>;

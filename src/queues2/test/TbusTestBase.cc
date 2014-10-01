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

#include <test/TbusTestBase.h>
#include "TbusDatarateQueueTester.h"

template <class T> TbusTestBase<T>::TbusTestBase(T* i) {
	instance = i;
}

template <class T> TbusTestBase<T>::~TbusTestBase()
{
	// TODO Auto-generated destructor stub
}

template <class T> void TbusTestBase<T>::addTest(functionPtr test) {
	tests.push(test);
}

template <class T> void TbusTestBase<T>::runNextTest() {
	if (!tests.empty()) {
		functionPtr test = tests.front();
		tests.pop();
		(instance->*test)();
	}
}

// Template instantiation for linker
template class TbusTestBase<TbusDatarateQueueTester>;

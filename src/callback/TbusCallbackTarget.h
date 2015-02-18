/*
 * TbusCallbackTarget.h
 *
 *  Created on: 18.02.2015
 *      Author: bialon
 */

#ifndef TBUSCALLBACKTARGET_H_
#define TBUSCALLBACKTARGET_H_

/**
 * Defines a Callback target.
 * No special methods need to be inherited.
 */
class TbusCallbackTarget {
	public:

		/**
		 * Provides a callback object.
		 * Every void-method with no parameters in TbusCallbackTarget is callable.
		 */
		template<typename T> class TbusCallback {
			typedef void(T::* callbackFunction)();

			private:
				T* object;
				callbackFunction method;

			public:
				TbusCallback(T* o, callbackFunction m) :
					object(o),
					method(m) {
				}

				void doCall() const {
					(object->*method)();
				}
		};
};

#endif /* TBUSCALLBACKTARGET_H_ */

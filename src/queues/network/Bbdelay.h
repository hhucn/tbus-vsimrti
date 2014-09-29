/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */
/**
 * Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#ifndef BBDELAY_H_
#define BBDELAY_H_

#include <string.h>
#include <omnetpp.h>

namespace projekt {

class Bbdelay {

	private:
		/**
		 * backbone delay for the simulation
		 */
		simtime_t bb;
		/**
		 * droprate from queue
		 */
		double dropValue;

	public:
		// constructor / destructor
		Bbdelay();
		Bbdelay* copy();
		~Bbdelay();

		// Output
		std::ostream& operator<<(std::ostream& os);

		// getter
		simtime_t bbdelay();
		double droprate();

		// setter
		void setBbdelay(simtime_t delay);
		void setDroprate(double rate);
};

} /* namespace projekt */
#endif /* BBDELAY_H_ */

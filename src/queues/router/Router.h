/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef ROUTER_H_
#define ROUTER_H_

#include <omnetpp.h>

namespace projekt {

	class Router : public cCompoundModule{
		private:

		public:
			Router();
			virtual ~Router();

		protected:
			virtual void initialize();
			virtual void handleMessage(cMessage* msg);
	};

} /* namespace projekt */
#endif /* ROUTER_H_ */

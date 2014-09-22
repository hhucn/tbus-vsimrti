/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <omnetpp.h>

namespace projekt {

class Queue : public cSimpleModule {

	private:
		cPacketQueue* queue;

	public:
		Queue();
		virtual ~Queue();

	protected:
		virtual void handleMessage(cMessage* msg);

};

} /* namespace projekt */
#endif /* QUEUE_H_ */

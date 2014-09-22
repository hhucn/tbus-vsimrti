/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef Routing_H_
#define Routing_H_

#include <omnetpp.h>
#include "MyPacket_m.h"
#include "Preamble.h"

namespace projekt {

class Routing : public cSimpleModule {

	private:
		// current network topology
		cTopology *topo;

		// defines mapping for routing table: destaddr -> gateindex
		typedef std::map<std::string, int> RoutingTable;

		// the real routing table
		RoutingTable rtable;

	public:
		Routing();
		virtual ~Routing();

	protected:
		virtual void handleMessage(cMessage* msg);

};
}
;

#endif /* Routing_H_ */

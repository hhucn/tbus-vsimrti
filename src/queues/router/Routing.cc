/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "Routing.h"

namespace projekt {

Define_Module(Routing)
;

Routing::Routing() {

	// which ned-files should be contained by topology
	std::vector<std::string> nedTypeNames;
	nedTypeNames.push_back("src.ned.InternetClient");
	nedTypeNames.push_back("src.ned.MobileClient");
	nedTypeNames.push_back("src.ned.Router");

	// print topology
	topo = new cTopology("topo");
	topo->extractByNedTypeName(nedTypeNames);
	EV<< "Size of topology is: " << topo->getNumNodes() << endl << endl;

	// crawling for information
	for (int i = 0; i < topo->getNumNodes(); i++) {
		cTopology::Node *node = topo->getNode(i);

		// print infos
		ev<< "Node i=" << i << " is " << node->getModule()->getFullPath()<<" (" << node << "). ";
		ev<< "It has " << node->getNumOutLinks() << " conns to other nodes ";
		ev<< "and " << node->getNumInLinks() << " conns from other nodes.\n";

		if (node->getNumOutLinks() > 0)
		ev<< "	Connections to other modules are:\n";

		// print gates
		for (int j = 0; j < node->getNumOutLinks(); j++) {
			cTopology::Node *neighbour = node->getLinkOut(j)->getRemoteNode();
			cGate *gate = node->getLinkOut(j)->getLocalGate();
			ev<< "	 - " << neighbour->getModule()->getFullPath()
			<< " through gate " << gate->getFullName() << endl;
		}

		// print submodules
		EV<<"	 - Submoduleiterator:" << endl;
		for (cModule::SubmoduleIterator s(node->getModule()); !s.end(); s++) {
			cModule *submodp = s();
			EV<<"	   - "<<submodp->getFullPath()<<" " <<submodp->getFullName()<<endl;
		}
	}
	EV<< endl;

	// finding this node
	cTopology::Node *thisNode;
	for (int i = 0; i < topo->getNumNodes(); i++) {
		std::size_t found1 = topo->getNode(i)->getModule()->getFullPath().find("router");
		std::size_t found2 = topo->getNode(i)->getModule()->getFullPath().find("Router");
		if (found1 != std::string::npos) thisNode = topo->getNode(i);
		else if (found2 != std::string::npos) thisNode = topo->getNode(i);
	}

	//
	// Brute force approach -- every node does topology discovery on its own,
	// and finds routes to all other nodes independently, at the beginning
	// of the simulation. This could be improved: (1) central routing database,
	// (2) on-demand route calculation
	//

	EV<< "Routing Table:"<<endl;

	// find and store next hops
	for (int i = 0; i < topo->getNumNodes(); i++) {
		if (topo->getNode(i) == thisNode) continue; // skip ourselves
		topo->calculateUnweightedSingleShortestPathsTo(topo->getNode(i));

		if (thisNode->getNumPaths() == 0) continue; // not connected

		cGate *parentModuleGate = thisNode->getPath(0)->getLocalGate();
		int gateIndex = parentModuleGate->getIndex();
		std::string address = topo->getNode(i)->getModule()->par(kPAR_IP);
		rtable[address] = gateIndex;
		EV<< "	- towards address " << address << " gateIndex is " << gateIndex << endl;
	}

	EV<< endl;
}

	/**
	 * default destructor
	 */
Routing::~Routing() {
	delete (topo);
}

void Routing::handleMessage(cMessage* msg) {
	// check for incoming MyPacket
	MyPacket* job = check_and_cast<MyPacket *>(msg);
	EV<< getFullPath() << ": routing from " << job->getSourceIP() << " to " << job->getDestinationIP() << endl;

	// iterate through routing table for finding right output gate
	RoutingTable::iterator it = rtable.find(job->getDestinationIP());
	if (it == rtable.end()) {
		EV<< getFullPath() << ": address " << job->getDestinationIP() << " unreachable, discarding packet " << msg << endl;
		bubble ("DISCARD");
		delete msg;
		return;
	}

	// getting gate and send all da packets
	int outGateIndex = (*it).second;
	EV<< getFullPath() << ": forwarding packet " << msg << " from " << job->getSourceIP() << " to ";
	EV<< job->getDestinationIP() << " on gate index " << outGateIndex << endl;

	// set timestamp
	((MyPacket*)job)->setTimestampsArraySize((1 + ((MyPacket*)job)->getTimestampsArraySize()));
	char timelog[50];
	sprintf(timelog, "%s %s,%.0f", kSTRING_ROUTER, kSTRING_ROUTED, simTime().dbl() * kMULTIPLIER_S_TO_NS);
	((MyPacket*)job)->setTimestamps((((MyPacket*)job)->getTimestampsArraySize() - 1), timelog);

	sendDelayed(msg, SIMTIME_ZERO, "gate$o", outGateIndex);
}
}
;

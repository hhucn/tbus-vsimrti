//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusInetNode.h"
#include "omnetpp.h"

Define_Module(TbusInetNode)

/**
 * Empty constructor.
 */
TbusInetNode::TbusInetNode() {}

/**
 * Empty destructor.
 */
TbusInetNode::~TbusInetNode() {}

/**
 * Simulation initialization.
 * Currently empty.
 * @param stage Initialization stage
 */
void TbusInetNode::initialize(int stage) {}

/**
 * Message handling.
 * Currently empty.
 * @param msg Message to handle
 */
void TbusInetNode::handleMessage(cMessage* msg) {}

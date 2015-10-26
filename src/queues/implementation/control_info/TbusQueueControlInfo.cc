//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include <control_info/TbusQueueControlInfo.h>

/**
 * Empty constructor
 */
TbusQueueControlInfo::TbusQueueControlInfo() {}

/**
 * Empty destructor
 */
TbusQueueControlInfo::~TbusQueueControlInfo() {}

/**
 * Time for earliest delivery from qeue
 * @return earliest delivery time
 */
simtime_t TbusQueueControlInfo::getEarliestDelivery() {
	return earliestDelivery;
}

/**
 * Arrival time at queue
 * @return arrival time
 */
simtime_t TbusQueueControlInfo::getQueueArrival() {
	return queueArrival;
}

/**
 * Time at which the packet became head of queue
 * @return head of queue time
 */
simtime_t TbusQueueControlInfo::getHeadOfQueue() {
	return headOfQueue;
}

/**
 * Setter for queue arrival time
 * @param time Time to be set for queue arrival
 */
void TbusQueueControlInfo::setQueueArrival(simtime_t time) {
	queueArrival = time;
}

/**
 * Setter for earliest delivery time
 * @param time Time to set for earliest delivery
 */
void TbusQueueControlInfo::setEarliestDelivery(simtime_t time) {
	earliestDelivery = time;
}

/**
 * Setter for head of queue time
 * @param time Time to set for head of queue
 */
void TbusQueueControlInfo::setHeadOfQueue(simtime_t time) {
	headOfQueue = time;
}

#include "TbusInetPHY.h"
#include "ChannelControl.h"

Define_Module(TbusInetPHY)

/**
 * Empty constructor.
 */
TbusInetPHY::TbusInetPHY() {}

/**
 * Empty destructor.
 */
TbusInetPHY::~TbusInetPHY() {}

/**
 * Simulation initialization.
 * - Stage 0: Gate id assignment, get references
 * - Stage 2: Register IP
 * @param stage Init stage level
 */
void TbusInetPHY::initialize(int stage) {
	ChannelAccess::initialize(stage);

	if (stage == 0) {
		upperLayerIn 	= findGate("upperLayerIn");
		upperLayerOut 	= findGate("upperLayerOut");
		radioIn 		= findGate("radioIn");

		tbusCC = check_and_cast<TbusChannelControl*>(ChannelControl::get());
	} else if (stage == 3) {
		// Register ip address at channel control
		tbusCC->registerIP(myHostRef);
	}
}

/**
 * Handle message from upper layer.
 * @param msg Message to handle
 */
void TbusInetPHY::handleUpperMessage(cMessage* msg) {
	sendToChannel(msg);
}

/**
 * Handle message from lower layer.
 * @param msg Message to handle
 */
void TbusInetPHY::handleLowerMessage(cMessage* msg) {
	send(msg, upperLayerOut);
}

/**
 * Handle incoming message.
 * @param msg Message to handle
 */
void TbusInetPHY::handleMessage(cMessage* msg) {
	EV << "TbusInetPHY received message on " << msg->getArrivalGate()->getName() << endl;
	if (msg->isSelfMessage()) {
		// Self message
		delete msg;
	} else if (msg->arrivedOn(upperLayerIn)) {
		// Message from upper layer
		handleUpperMessage(msg);
	} else {
		// Message from lower layer
		handleLowerMessage(msg);
	}
}

/**
 * Send message to channel via channel control.
 * @param msg Message to send
 */
void TbusInetPHY::sendToChannel(cMessage* msg) {
	tbusCC->sendToChannel(msg, myHostRef);
}

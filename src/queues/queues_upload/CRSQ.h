/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */
/**
 * Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#ifndef CRSQ_H_
#define CRSQ_H_

#include "MyPacket_m.h"
#include "SendHeadAndDeletePacket_m.h"
#include "Preamble.h"
#include "IDatarateQueue.h"
#include "IQueue.h"

#include <omnetpp.h>
#include <Datarate.h>

namespace projekt {

class CRSQ : public cSimpleModule, public IQueue, public IDatarateQueue  {

	private:
		// signals
		simsignal_t signalDatarate, signalLossprob, signalLossprobRnd, signalPacketDatarateDelay, signalDropMeasured, signalDropSimulated;

		// remaining buffer in queue
		int freeBufferInByte;

		// our packet queue
		cPacketQueue *queue;

		// current self message for dispatching head of queue
		SendHeadAndDeletePacket *currSendHeadCRSQPacket;

		//some initialization value to overcome null pointer exceptions that happen at first dataratechange event
		Datarate* currentDatarate;

		//store all datarate characteristics since the last dispatch
		std::vector<Datarate*> datarateVector;

		//and store the timestamps (in nanoseconds) of the characteristics
		std::vector<simtime_t> drTimeVector;

		//how many bytes went over the air since the last dispatch
		double capacityBytes;

		//current loss probability
		double currentLossProbability;

		// current latency
		simtime_t currentLatency;

	public:
		CRSQ();
		~CRSQ();
		void datarateChanged(Datarate* newDatarate);

	protected:
		void initialize();

		void handleMessage(cMessage* msg);
		void handleSendHeadAndDeletePacket(SendHeadAndDeletePacket* sendHeadPacket);

		void scheduleNewSendHeadAndDeletePacket(simtime_t delay);
		void manipulateSelfMessageProcess();

		void addPacketToQueue(cMessage* msg);

		double calculateLossProbability();
		simtime_t calculateDatarateDelay(double packetsize);

		void transmitOrLooseHead(bool retBool[]);
		void dispatch(bool retBool[]);
};

};

#endif /* CRSQ_H_ */

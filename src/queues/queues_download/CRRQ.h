/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */
/**
 * Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#ifndef CRRQ_H_
#define CRRQ_H_

#include "MyPacket_m.h"
#include "SendHeadAndDeletePacket_m.h"
#include "Preamble.h"
#include "IDatarateQueue.h"
#include "IQueue.h"

#include <omnetpp.h>
#include <Datarate.h>

namespace projekt {

class CRRQ : public cSimpleModule, public IQueue, public IDatarateQueue {

	private:
		// signals
		simsignal_t signalDatarate, signalLossprob, signalLossprobRnd, signalPacketDatarateDelay, signalDropMeasured, signalDropSimulated;

		// remaining buffer in queue
		int freeBufferInByte;

		// our packet queue
		cPacketQueue *queue;

		// current self message for dispatching head of queue
		SendHeadAndDeletePacket *currSendHeadCRRQPacket;

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
		CRRQ();
		virtual ~CRRQ();
		virtual void datarateChanged(Datarate* newDatarate);
		friend class CRRQ_test; // need for tests

	protected:
		virtual void initialize();

		virtual void handleMessage(cMessage* msg);
		virtual void handleSendHeadAndDeletePacket(SendHeadAndDeletePacket* sendHeadPacket);
		virtual void handleControlMessage(cMessage* msg);

		virtual void scheduleNewSendHeadAndDeletePacket();
		virtual void scheduleNewSendHeadAndDeletePacket(simtime_t delay);
		virtual void manipulateSelfMessageProcess();

		virtual void addPacketToQueue(MyPacket* job);

		virtual double calculateLossProbability();
		virtual simtime_t calculateDatarateDelay(double packetsize);

		virtual void transmitOrLooseHead(bool retBool[]);
		virtual void dispatch(bool retBool[]);
}
;
}
;

#endif /* CRRQ_H_ */

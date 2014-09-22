/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef LOGPACKETS_H_
#define LOGPACKETS_H_

#include <omnetpp.h>

namespace projekt {

	class LogPackets {
		private:
			// local variables
			simtime_t sendTstamp;
			simtime_t recTstamp;
			int seqNumber;
			int packetNumber;
			int probePackets;
			int payload;
			bool dropped;

		public:
			// destrucor / constructor
			LogPackets();
			LogPackets(simtime_t sendStamp, simtime_t recStamp, int seqNr, int packetNr, int probePacketNr, int size,
					bool packetDropped);
			virtual ~LogPackets();

			// getter
			virtual simtime_t normSendTstamp();
			virtual simtime_t normRecTstamp();
			virtual int seqNr();
			virtual int packetNr();
			virtual int numberOfProbePackets();
			virtual int payloadsize();
			virtual bool boolDroppedMeasured();

			// setter
			virtual void setNormSendTstamp(simtime_t tstamp);
			virtual void setNormRecTstamp(simtime_t tstamp);
			virtual void setSeqNr(int nr);
			virtual void setPacketNr(int nr);
			virtual void setNumberOfProbePackets(int nr);
			virtual void setPayloadsize(int size);
			virtual void setBoolDroppedMeasured(bool packetDropped);

//			virtual void print();

	};

} /* namespace projekt */
#endif /* LOGPACKETS_H_ */

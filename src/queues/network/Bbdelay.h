/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */
/**
 * Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#ifndef BBDELAY_H_
#define BBDELAY_H_

#include <string.h>
#include <omnetpp.h>

namespace projekt {

class Bbdelay {

	private:
		/**
		 * backbone delay for the simulation
		 */
		simtime_t bb;
		/**
		 * simulated backbone delay for the simulation
		 */
		simtime_t bbSim;
		/**
		 * is the bbdelay valid?
		 */
		bool validbb;
		/**
		 * delay of first packet in train
		 */
		simtime_t del;
		/**
		 * the technology used by the modem the time the bbdelay was measured
		 */
		std::string modem;
		/**
		 * the sequence number of the packets this measurement derived from this is
		 * an optional attribut
		 */
		int seqNumber;
		/**
		 * the time at which the bbedelay change get's effective
		 */
		simtime_t stamp;
		/**
		 * geoposition
		 */
		double lng;
		double lat;
		/**
		 * some counter
		 */
		int msgNumberProbePackets;
		int msgPayloadSize;
		/*
		 * droprate from queue
		 */
		double dropValue;

	public:
		// constructor / destructor
		Bbdelay();
		Bbdelay(simtime_t bbdelay, bool validbbdelay, std::string modemMode);
		Bbdelay(simtime_t bbdelay, double longitude, double latitude, bool validbbdelay, std::string modemMode, int sequenceNumber, simtime_t tstamp);
		virtual Bbdelay* copy();
		virtual ~Bbdelay();

		// getter
		virtual simtime_t bbdelay();
		virtual simtime_t bbdelaySim();
		virtual std::string modemMode();
		virtual bool validbbdelay();
		virtual int sequenceNumber();
		virtual double longitude();
		virtual double latitude();
		virtual simtime_t timeStamp();
		virtual std::string toString(std::string s);
		virtual int cMsgNumberOfProbePackets();
		virtual int cMsgPayloadSize();
		virtual simtime_t delay();
		virtual double droprate();

		// setter
		virtual void setBbdelay(simtime_t delay);
		virtual void setBbdelaySim(simtime_t delaySim);
		virtual void setValidBbdelay(bool valid);
		virtual void setModemMode(std::string modemMode);
		virtual void setSequenceNumber(int nr);
		virtual void setTimeStamp(simtime_t tstamp);
		virtual void setLongitude(double l);
		virtual void setLatitude(double l);
		virtual void setDelay(simtime_t d);
		virtual void setCMsgNumberOfProbePackets(int count);
		virtual void setCMsgPayloadSize(int count);
		virtual void setDroprate(double rate);
};

} /* namespace projekt */
#endif /* BBDELAY_H_ */

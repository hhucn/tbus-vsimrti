/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef DATARATE_H_
#define DATARATE_H_

#include <string.h>
#include <omnetpp.h>

namespace projekt {

class Datarate {

	private:
		/**
		 * data rate in bytes/s
		 */
		double data;
		/**
		 * simulated data rate in bytes/s
		 */
		double dataSim;
		/**
		 * is the data rate valid?
		 */
		bool validdata;
		/**
		 * the drop probability for packets
		 */
		double drop;
		/**
		 * the technology used by the modem the time this datarate measurement was
		 * generated
		 */
		std::string modem;
		/**
		 * the sequence number of the packets that were used to derive the
		 * measurement this value is optional and only needed for statistics and not
		 * for the actual simulation
		 */
		int seqNumber;
		/**
		 * the time at which the datarate change get's effective
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

	public:
		// constructor / destructor
		Datarate();
		Datarate(double datarate, bool validdatarate, double droprate, std::string modemMode);
		Datarate(double datarate, double longitude, double latitude, bool validdatarate, double droprate, std::string modemMode, int sequenceNumber,
				simtime_t tstamp);
		virtual ~Datarate();

		// getter
		virtual Datarate* copy();
		virtual double datarate();
		virtual double datarateSim();
		virtual double droprate();
		virtual double longitude();
		virtual double latitude();
		virtual bool validdatarate();
		virtual int sequenceNumber();
		virtual int compareTo(Datarate* o);
		virtual simtime_t tstamp();
		virtual std::string modemMode();
		virtual std::string toString(std::string s);
		virtual int cMsgNumberOfProbePackets();
		virtual int cMsgPayloadSize();

		// setter
		virtual void setDatarate(double rate);
		virtual void setDatarateSim(double rateSim);
		virtual void setDroprate(double rate);
		virtual void setValidDatarate(bool valid);
		virtual void setSequenceNumber(int nr);
		virtual void setModemMode(std::string modemMode);
		virtual void setTimeStamp(simtime_t tstamp);
		virtual void setLongitude(double l);
		virtual void setLatitude(double l);
		virtual void setCMsgNumberOfProbePackets(int count);
		virtual void setCMsgPayloadSize(int count);
};

} /* namespace projekt */
#endif /* DATARATE_H_ */

/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef LOGBBDELAY_H_
#define LOGBBDELAY_H_

#include <omnetpp.h>

namespace projekt {

	class LogBbdelay {
		private:
			// all local variables
			simtime_t tstamp;
			double latitudeData;
			double longitudeData;
			simtime_t bbdelay;
			simtime_t bbdelaySim;
			bool validData;
			double delayData;
			double dropdateData;
			int seqNumber;
			std::string modemModeData;
			int trainsizeData;
			int packetsizeData;

		public:
			// constructor / destructor
			LogBbdelay();
			LogBbdelay(simtime_t stamp, double latitude, double longitude, simtime_t bbdelay, simtime_t bbdelaySim,
					bool valid, long delay, double droprate, int seqNr, std::string modemMode, int trainSize, int packetSize);
			virtual ~LogBbdelay();

			// getter
			virtual simtime_t normTstamp();
			virtual double latitude();
			virtual double longitude();
			virtual simtime_t backboneDelay();
			virtual simtime_t backboneDelaySim();
			virtual bool valid();
			virtual long delay();
			virtual double droprate();
			virtual int seqNr();
			virtual std::string modemMode();
			virtual int trainsize();
			virtual int packetsize();

			// setter
			virtual void setNormTstamp(simtime_t stamp);
			virtual void setLatitude(double latitude);
			virtual void setLongitude(double longitude);
			virtual void setBackboneDelay(simtime_t backboneDelay);
			virtual void setBackboneDelaySim(simtime_t backboneDelaySim);
			virtual void setValid(bool valid);
			virtual void setDelay(long delay);
			virtual void setDroprate(double droprate);
			virtual void setSeqNr(int seqNr);
			virtual void setModemMode(std::string modemMode);
			virtual void setTrainsize(int trainSize);
			virtual void setPacketsize(int packetSize);
	};

} /* namespace projekt */
#endif /* LOGBBDELAY_H_ */

/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#ifndef LOGDATARATE_H_
#define LOGDATARATE_H_

#include <omnetpp.h>

namespace projekt {

	class LogDatarate {
		private:
			// local variables
			simtime_t tstamp;
			double latitudeData;
			double longitudeData;
			long data;
			long dataSim;
			bool validData;
			double droprateData;
			int seqNumber;
			std::string modemModeData;
			int trainsizeData;
			int packetsizeData;

		public:
			// constructor / destrcutor
			LogDatarate();
			LogDatarate(simtime_t stamp, double latitude, double longitude, long datarate, long datarateSim, bool valid, double droprate, int seqNr, std::string modemMode, int trainSize, int packetSize);
			virtual ~LogDatarate();

			// getter
			virtual simtime_t normTstamp();
			virtual double latitude();
			virtual double longitude();
			virtual long datarate();
			virtual long datarateSim();
			virtual bool valid();
			virtual double droprate();
			virtual int seqNr();
			virtual std::string modemMode();
			virtual int trainsize();
			virtual int packetsize();

			// stter
			virtual void setNormTstamp(simtime_t stamp);
			virtual void setLatitude(double latitude);
			virtual void setLongitude(double longitude);
			virtual void setDatarate(long datarate);
			virtual void setDatarateSim(long datarateSim);
			virtual void setValid(bool valid);
			virtual void setDroprate(double droprate);
			virtual void setSeqNr(int seqNr);
			virtual void setModemMode(std::string modemMode);
			virtual void setTrainsize(int trainSize);
			virtual void setPacketsize(int packetSize);
	};

} /* namespace projekt */
#endif /* LOGDATARATE_H_ */

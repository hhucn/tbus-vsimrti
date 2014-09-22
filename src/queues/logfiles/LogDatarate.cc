/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "LogDatarate.h"

namespace projekt {

/**
 * default constructor for datarate, everything will be 0, "" or false
 */
LogDatarate::LogDatarate() {
	setNormTstamp(0);
	setLatitude(0);
	setLongitude(0);
	setDatarate(0);
	setDatarateSim(0);
	setValid(false);
	setDroprate(0);
	setSeqNr(0);
	setModemMode("-");
	setTrainsize(0);
	setPacketsize(0);
}

/**
 * custom constructor for datarate
 * @param stamp, current timestamp as simtime_t
 * @param latitude as double
 * @param longitude as double
 * @param datarate as double
 * @param datarateSim as double
 * @param valid, true when valid
 * @param droprate as double
 * @param seqNr as int from current MyPacket
 * @param modemMode as std::string from current MyPacket
 * @param trainSize as int from current MyPacket
 * @param packetSize as int from current MyPacket
 */
LogDatarate::LogDatarate(simtime_t stamp, double latitude, double longitude, long datarate, long datarateSim,
		bool valid, double droprate, int seqNr, std::string modemMode, int trainSize, int packetSize) {
	setNormTstamp(stamp);
	setLatitude(latitude);
	setLongitude(longitude);
	setDatarate(datarate);
	setDatarateSim(datarateSim);
	setValid(valid);
	setDroprate(droprate);
	setSeqNr(seqNr);
	setModemMode(modemMode);
	setTrainsize(trainSize);
	setPacketsize(packetSize);
}

/**
 * default destrcutor
 */
LogDatarate::~LogDatarate() {
}


/**
 * Get normalized time stamp
 * @return simtime_t
 */
simtime_t LogDatarate::normTstamp() {
	return tstamp;
}

/**
 * Get latitude from current MyPacket
 * @return double
 */
double LogDatarate::latitude() {
	return latitudeData;
}

/**
 * Get lontigute from current MyPacket
 * @return double
 */
double LogDatarate::longitude() {
	return longitudeData;
}

/**
 * Get current datarate
 * @return long
 */
long LogDatarate::datarate() {
	return data;
}

/**
 * Get current sim datarate
 * @return long
 */
long LogDatarate::datarateSim() {
	return dataSim;
}

/**
 * True, when valid
 * @return bool
 */
bool LogDatarate::valid() {
	return validData;
}

/**
 * Get current droprate from current MyPacket
 * @return double
 */
double LogDatarate::droprate() {
	return droprateData;
}

/**
 * Get current sequence number from current MyPacket
 * @return int
 */
int LogDatarate::seqNr() {
	return seqNumber;
}

/**
 * Get current modem mode from current MyPacket
 * @return std::string
 */
std::string LogDatarate::modemMode() {
	return modemModeData;
}

/**
 * Get trainsize from current MyPacket
 * @return int
 */
int LogDatarate::trainsize() {
	return trainsizeData;
}

/**
 * Get packetsize from current MyPacket
 * @return int
 */
int LogDatarate::packetsize() {
	return packetsizeData;
}


/**
 * Set normalized timestamp
 * @param stamp as simtime_t
 */
void LogDatarate::setNormTstamp(simtime_t stamp) {
	tstamp = stamp;
}

/**
 *Set latitude from current MyPacket
 * @param latitude as double
 */
void LogDatarate::setLatitude(double latitude) {
	latitudeData = latitude;
}

/**
 * Set longitude from current MyPacket
 * @param longitude as double
 */
void LogDatarate::setLongitude(double longitude) {
	longitudeData = longitude;
}

/**
 * Set datarate
 * @param datarate as long
 */
void LogDatarate::setDatarate(long datarate) {
	data = datarate;
}

/**
 * Set sim datarate
 * @param datarateSim as long
 */
void LogDatarate::setDatarateSim(long datarateSim) {
	dataSim = datarateSim;
}

/**
 * Set valid-boolean
 * @param valid as bool
 */
void LogDatarate::setValid(bool valid) {
	validData = valid;
}

/**
 * Set droprate from current MyPacket
 * @param droprate as double
 */
void LogDatarate::setDroprate(double droprate) {
	droprateData = droprate;
}

/**
 * Set sequence number from current MyPacket
 * @param seqNr as int
 */
void LogDatarate::setSeqNr(int seqNr) {
	seqNumber = seqNr;
}

/**
 * Set modem mode from current MyPacket
 * @param modemMode as std::string
 */
void LogDatarate::setModemMode(std::string modemMode) {
	modemModeData = modemMode;
}

/**
 * Set trainsize from current MyPacket
 * @param trainSize as int
 */
void LogDatarate::setTrainsize(int trainSize) {
	trainsizeData = trainSize;
}

/**
 * Set packetsize from current MyPacket
 * @param packetSize as int
 */
void LogDatarate::setPacketsize(int packetSize) {
	packetsizeData = packetSize;
}

} /* namespace projekt */

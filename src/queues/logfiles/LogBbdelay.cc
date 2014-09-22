/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "LogBbdelay.h"

namespace projekt {

/**
 * default constructor, all values are empty, 0, false,...
 * logs all needed backbone values for a packet
 */
LogBbdelay::LogBbdelay() {
	setNormTstamp(0);
	setLatitude(0);
	setLongitude(0);
	setBackboneDelay(0);
	setBackboneDelaySim(0);
	setValid(false);
	setDelay(0);
	setDroprate(0);
	setSeqNr(0);
	setModemMode("-");
	setTrainsize(0);
	setPacketsize(0);

}

/**
 * constructor for initializing
 * logs all needed backbone values for a packet
 * @param stamp, simulation time for this object as simtime_t
 * @param latitude, geoposition as double from MyPacket
 * @param longitude, geoposition as double from MyPacket
 * @param bbdelay, backbone delay as simtime_t, which the MyPacket has to wait
 * @param bbdelaySim, simulated backbone delay as simtime, which the MyPacket has to wait
 * @param valid, is this delay true?
 * @param delay, current delay from MyPacket
 * @param droprate, droprate from MyPacket
 * @param seqNr, sequence number for current MyPacket
 * @param modemMode, delivered modem mode
 * @param trainSize, trainsize from current MyPacket
 * @param packetSize, packet size from current MyPacket
 */
LogBbdelay::LogBbdelay(simtime_t stamp, double latitude, double longitude, simtime_t bbdelay, simtime_t bbdelaySim,
		bool valid, long delay, double droprate, int seqNr, std::string modemMode, int trainSize, int packetSize) {
	setNormTstamp(stamp);
	setLatitude(latitude);
	setLongitude(longitude);
	setBackboneDelay(bbdelay);
	setBackboneDelaySim(bbdelaySim);
	setValid(valid);
	setDelay(delay);
	setDroprate(droprate);
	setSeqNr(seqNr);
	setModemMode(modemMode);
	setTrainsize(trainSize);
	setPacketsize(packetSize);
}

/**
 * destructor
 */
LogBbdelay::~LogBbdelay() {
}

/**
 * Returns timestamp from backbone delay
 * @return simtime_t
 */
simtime_t LogBbdelay::normTstamp() {
	return tstamp;
}

/**
 * Returns latitude from MyPacket with this backbone delay
 * @return simtime_t
 */
double LogBbdelay::latitude() {
	return latitudeData;
}

/**
 * Returns longitude from MyPacket with this backbone delay
 * @return double
 */
double LogBbdelay::longitude() {
	return longitudeData;
}

/**
 * Returns backbone delay from MyPacket
 * @return
 */
simtime_t LogBbdelay::backboneDelay() {
	return bbdelay;
}

/**
 * Returns backbone delay sim from MyPacket
 * @return simtime_t
 */
simtime_t LogBbdelay::backboneDelaySim() {
	return bbdelaySim;
}

/**
 * Returns weather this is a valid delay or not
 * @return bool
 */
bool LogBbdelay::valid() {
	return validData;
}

/**
 * Current delay from MyPacket with this backbone delay
 * @return long
 */
long LogBbdelay::delay() {
	return delayData;
}

/**
 * Current droprate from MyPacket with this backbone delay
 * @return double
 */
double LogBbdelay::droprate() {
	return dropdateData;
}

/**
 * Current seqNr from MyPacket with this backbone delay
 * @return int
 */
int LogBbdelay::seqNr() {
	return seqNumber;
}

/**
 * Current modemMode from MyPacket with this backbone delay
 * @return std::string
 */
std::string LogBbdelay::modemMode() {
	return modemModeData;
}

/**
 * Current trainsize from MyPacket with this backbone delay
 * @return
 */
int LogBbdelay::trainsize() {
	return trainsizeData;
}

/**
 * Current packetsize from MyPacket with this backbone delay
 * @return
 */
int LogBbdelay::packetsize() {
	return packetsizeData;
}



/**
 * Set norm timestamp
 * @param stamp as simtime_t
 */
void LogBbdelay::setNormTstamp(simtime_t stamp) {
	tstamp = stamp;
}

/**
 * Set latitude
 * @param latitude as double
 */
void LogBbdelay::setLatitude(double latitude) {
	latitudeData = latitude;
}

/**
 * Set Longitude
 * @param longitude as double
 */
void LogBbdelay::setLongitude(double longitude) {
	longitudeData = longitude;
}

/**
 * Set backbone delay
 * @param backboneDelay as simtime_t
 */
void LogBbdelay::setBackboneDelay(simtime_t backboneDelay) {
	bbdelay = backboneDelay;
}

/**
 * Set backbone delay sim
 * @param backboneDelaySim as simtime_t
 */
void LogBbdelay::setBackboneDelaySim(simtime_t backboneDelaySim) {
	bbdelaySim = backboneDelaySim;
}

/**
 * Set valid
 * @param valid as bool
 */
void LogBbdelay::setValid(bool valid) {
	validData = valid;
}

/**
 * Set delay
 * @param delay as long
 */
void LogBbdelay::setDelay(long delay) {
	delayData = delay;
}

/**
 * Set droprate
 * @param droprate as double
 */
void LogBbdelay::setDroprate(double droprate) {
	dropdateData = droprate;
}

/**
 * Set sequence number
 * @param seqNr as seqNr
 */
void LogBbdelay::setSeqNr(int seqNr) {
	seqNumber = seqNr;
}

/**
 * Set modem mode
 * @param modemMode as std::string
 */
void LogBbdelay::setModemMode(std::string modemMode) {
	modemModeData = modemMode;
}

/**
 * Set trainsize
 * @param trainSize as int
 */
void LogBbdelay::setTrainsize(int trainSize) {
	trainsizeData = trainSize;
}

/**
 * Set packetsize
 * @param packetSize as int
 */
void LogBbdelay::setPacketsize(int packetSize) {
	packetsizeData = packetSize;
}

} /* namespace projekt */

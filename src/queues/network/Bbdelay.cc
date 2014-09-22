/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "Bbdelay.h"

namespace projekt {

/**
 * default constructor
 * seqnumber and stamp equals -1
 */
Bbdelay::Bbdelay() {
	seqNumber = -1;
	stamp = -1;
}

/**
 * default destructor
 */
Bbdelay::~Bbdelay() {
}

/**
 * Constructor
 * @param bbdelay as simtime_t
 * @param validbbdelay as validbbdelay
 * @param modemMode as std::string
 */
Bbdelay::Bbdelay(simtime_t bbdelay, bool validbbdelay, std::string modemMode) {
	setBbdelay(bbdelay);
	setModemMode(modemMode);
	setValidBbdelay(validbbdelay);
}

/**
 * Constructor
 * @param bbdelay as simtime_t
 * @param validbbdelay as validbbdelay
 * @param modemMode as std::string
 * @param sequenceNumber as int
 * @param tstamp as simtime_t
 */
Bbdelay::Bbdelay(simtime_t bbdelay, double longitude, double latitude, bool validbbdelay, std::string modemMode, int sequenceNumber, simtime_t tstamp) {
	setBbdelay(bbdelay);
	setLongitude(longitude);
	setLatitude(latitude);
	setModemMode(modemMode);
	setValidBbdelay(validbbdelay);
	setSequenceNumber(sequenceNumber);
	setTimeStamp(tstamp);
}

/**
 * Returns a new Bbdelay, pointed to current values
 * @return Bbdelay*
 */
Bbdelay* Bbdelay::copy() {
	return new Bbdelay(*this);
}

/**
 * Get current backbone delay
 * @return simtime_t
 */
simtime_t Bbdelay::bbdelay() {
	return bb;
}

/**
 * Get current simulated backbone delay
 * @return simtime_t
 */
simtime_t Bbdelay::bbdelaySim() {
	return bbSim;
}

/**
 * Get modem mode
 * @return std::string
 */
std::string Bbdelay::modemMode() {
	return modem;
}

/**
 * Returns true, when Bbdelay is valid
 * @return bool
 */
bool Bbdelay::validbbdelay() {
	return validbb;
}

/**
 * Get sequence number from current MyPacket*
 * @return int
 */
int Bbdelay::sequenceNumber() {
	return seqNumber;
}

/**
 * Get longitude
 * @return double
 */
double Bbdelay::longitude(){
	return lng;
}

/**
 * get latitude
 * @return
 */
double Bbdelay::latitude(){
	return lat;
}

/**
 * returns the timestamp at which the datarate get's set as the value is
 * optional it might be -1 if it is not defined in the simulation
 *
 * @return simtime_t
 */
simtime_t Bbdelay::timeStamp() {
	return stamp;
}

/**
 * Returns all important informations as string
 * @param s
 * @return std::string
 */
std::string Bbdelay::toString(std::string s) {
	char buff[100];
	sprintf(buff, "%s%.6f%s%s%s%s%s", "(", bb.dbl(), ",", (validbb) ? "true" : "false", ",", modem.c_str(), ")");
	s = buff;
	return s;
}

/**
 *
 * @return
 */
int Bbdelay::cMsgNumberOfProbePackets(){
	return msgNumberProbePackets;
}

/**
 *
 * @return
 */
int Bbdelay::cMsgPayloadSize(){
	return msgPayloadSize;
}

/**
 *
 * @return
 */
simtime_t Bbdelay::delay(){
	return del;
}

/**
 *
 * @return
 */
double Bbdelay::droprate(){
	return dropValue;
}

/**
 * Set current backbone delay
 * @param delay
 */
void Bbdelay::setBbdelay(simtime_t delay) {
	bb = delay;
}

/**
 * Set current simulated backbone delay
 * @param delay
 */
void Bbdelay::setBbdelaySim(simtime_t delaySim) {
	bbSim = delaySim;
}

/**
 * Set longitude
 * @param longitude
 */
void Bbdelay::setLongitude(double l){
	lng = l;
}

/**
 * Set latitude
 * @param latitude
 */
void Bbdelay::setLatitude(double l){
	lat = l;
}

/**
 * Set true, when bbdelay is valid
 * @param valid
 */
void Bbdelay::setValidBbdelay(bool valid) {
	validbb = valid;
}

/**
 * Set current modem mode
 * @param modemMode
 */
void Bbdelay::setModemMode(std::string modemMode) {
	modem = modemMode;
}

/**
 * Set current sequence number from current MyPacket*
 * @param nr
 */
void Bbdelay::setSequenceNumber(int nr) {
	seqNumber = nr;
}

/**
 * Set current timestamp
 * @param tstamp
 */
void Bbdelay::setTimeStamp(simtime_t tstamp) {
	stamp = tstamp;
}

/**
 *
 * @param count
 */
void Bbdelay::setCMsgNumberOfProbePackets(int count){
	msgNumberProbePackets = count;
}

/**
 *
 * @param count
 */
void Bbdelay::setCMsgPayloadSize(int count){
	msgPayloadSize = count;
}

/**
 *
 * @param d
 */
void Bbdelay::setDelay(simtime_t d){
	del = d;
}

/**
 *
 * @param rate
 */
void Bbdelay::setDroprate(double rate){
	dropValue = rate;
}

} /* namespace projekt */

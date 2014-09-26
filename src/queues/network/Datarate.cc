/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */
/**
 * Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#include "Datarate.h"

namespace projekt {

/**
 * default constructor
 * sequence number and stamp will be -1
 */
Datarate::Datarate() {
	this->seqNumber = -1;
	this->stamp = -1;
}

/**
 * default destructor
 */
Datarate::~Datarate() {
}

/**
 * This object represents datarate a and droprate network characteristics
 *
 * @param datarate [bytes/s]
 * @param validdatarate [true = valid estimate]
 * @param droprate [0.0 , 1.0]
 * @param modemMode String representing the network technology used during
 * measurement
 */
Datarate::Datarate(double datarate, bool validdatarate, double droprate, std::string modemMode) {
	this->data = datarate;
	this->validdata = validdatarate;
	this->drop = droprate;
	this->modem = modemMode;
}

/**
 * This object represents datarate a and droprate network characteristics
 *
 * @param datarate [bytes/s]
 * @param validdatarate [true = valid estimate]
 * @param droprate [0.0 , 1.0]
 * @param modemMode String representing the network technology used during
 * measurement
 */
Datarate::Datarate(double datarate, double longitude, double latitude, bool validdatarate, double droprate, std::string modemMode, int sequenceNumber, simtime_t tstamp) {
	setDatarate(datarate);
	setLongitude(longitude);
	setLatitude(latitude);
	setValidDatarate(validdatarate);
	setDroprate(droprate);
	setModemMode(modemMode);
	setSequenceNumber(sequenceNumber);
	setTimeStamp(tstamp);
}

/**
 * Returns new Datarate* with this values
 * @return Datarate*
 */
Datarate* Datarate::copy() {
	return new Datarate(*this);
}

/**
 * Return the datarate in bytes/s
 * @return double
 */
double Datarate::datarate() {
	return data;
}

/**
 * Return the simulated datarate in bytes/s
 * @return double
 */
double Datarate::datarateSim() {
	return dataSim;
}

/**
 * Return the dropprobability in % [0.0 , 1.0]
 * @return double
 */
double Datarate::droprate() {
	return drop;
}

/**
 * Get longitude
 * @return double
 */
double Datarate::longitude(){
	return lng;
}

/**
 * get latitude
 * @return
 */
double Datarate::latitude(){
	return lat;
}

/**
 * Return the networktechnology used during measurement of this performance data
 * @return std::string
 */
std::string Datarate::modemMode() {
	return modem;
}

/**
 * Was the datarate estimation gathered in a "correct" way or not
 * @return true, when valid
 */
bool Datarate::validdatarate() {
	return validdata;
}

/**
 * returns the sequence number of the packets that were used to generate
 * this measurement
 * @return int
 */
int Datarate::sequenceNumber() {
	return seqNumber;
}

/**
 * returns the timestamp at which the datarate get's set as the value is
 * optional it might be -1 if it is not defined in the simulation
 * @return simtime_t
 */
simtime_t Datarate::tstamp() {
	return stamp;
}

/**
 * Add a toString function for debugging purposes
 * @return std::string
 */
std::string Datarate::toString(std::string s) {
	char buff[100];
	sprintf(buff, "%s%gkB/s", "(", data / 1000);
	sprintf(buff, "%s%s%s", buff, ", ", (validdata) ? "true" : "false");
	sprintf(buff, "%s%s%g", buff, ", ", drop);
	sprintf(buff, "%s%s%s%s", buff, ", ", modem.c_str(), ")");
	s = buff;
	return s;
}

/**
 * compares via sequence number!
 * @param cmpData, comparable Datarate*
 * @return int
 */
int Datarate::compareTo(Datarate* cmpData) {
	if (cmpData == NULL) {
		return -1;
	}
	return (sequenceNumber() < cmpData->sequenceNumber() ? -1 : 1);
}

/**
 *
 * @return
 */
int Datarate::cMsgNumberOfProbePackets(){
	return msgNumberProbePackets;
}

/**
 *
 * @return
 */
int Datarate::cMsgPayloadSize(){
	return msgPayloadSize;
}

/**
 * Set current datarate
 * @param rate
 */
void Datarate::setDatarate(double rate) {
	data = rate;
}

/**
 * Set current simulated datarate
 * @param rate
 */
void Datarate::setDatarateSim(double rateSim) {
	dataSim = rateSim;
}

/**
 * Set current droprate
 * @param rate
 */
void Datarate::setDroprate(double rate) {
	drop = rate;
}

/**
 * Set longitude
 * @param longitude
 */
void Datarate::setLongitude(double l){
	lng = l;
}

/**
 * Set latitude
 * @param latitude
 */
void Datarate::setLatitude(double l){
	lat = l;
}

/**
 * True, when datarate is valid
 * @param valid
 */
void Datarate::setValidDatarate(bool valid) {
	validdata = valid;
}

/**
 * Set current sequence number
 * @param nr
 */
void Datarate::setSequenceNumber(int nr) {
	seqNumber = nr;
}

/**
 * Set current modem mode
 * @param modemMode
 */
void Datarate::setModemMode(std::string modemMode) {
	modem = modemMode;
}

/**
 * Set current timestamp
 * @param tstamp
 */
void Datarate::setTimeStamp(simtime_t tstamp) {
	stamp = tstamp;
}

/**
 *
 * @param count
 */
void Datarate::setCMsgNumberOfProbePackets(int count){
	msgNumberProbePackets = count;
}

/**
 *
 * @param count
 */
void Datarate::setCMsgPayloadSize(int count){
	msgPayloadSize = count;
}

} /* namespace projekt */

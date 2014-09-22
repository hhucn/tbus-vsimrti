/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

#include "LogPackets.h"

namespace projekt {

/**
 * custrom constructor, everything will be 0 and booldropped is true
 */
LogPackets::LogPackets() {
	setNormSendTstamp(0);
	setNormRecTstamp(0);
	setSeqNr(0);
	setPacketNr(0);
	setNumberOfProbePackets(0);
	setPayloadsize(0);
	setBoolDroppedMeasured(true);
}

/**
 * default constructor
 */
LogPackets::~LogPackets() {
}

/**
 * Custrom constructor
 * @param sendStamp, send timestamp in first node as simtime_t
 * @param recStamp, receive timestamo from alst node as simtime_t
 * @param seqNr, current sequence number as int
 * @param packetNr, current packet packet number as int
 * @param probePacketNr, current probe packet nr as int
 * @param size, current sie as int
 * @param packetDropped, true, when dropped
 */
LogPackets::LogPackets(simtime_t sendStamp, simtime_t recStamp, int seqNr, int packetNr, int probePacketNr, int size,
		bool packetDropped) {
	setNormSendTstamp(sendStamp);
	setNormRecTstamp(recStamp);
	setSeqNr(seqNr);
	setPacketNr(packetNr);
	setNumberOfProbePackets(probePacketNr);
	setPayloadsize(size);
	setBoolDroppedMeasured(packetDropped);
}

/**
 * Returns send time in first node
 * @return simtime_t
 */
simtime_t LogPackets::normSendTstamp() {
	return sendTstamp;
}

/**
 * Returns receive time in first node
 * @return simtime_t
 */
simtime_t LogPackets::normRecTstamp() {
	return recTstamp;
}

/**
 * Returns sequence number
 * @return int
 */
int LogPackets::seqNr() {
	return seqNumber;
}

/**
 * Returns packet number
 * @return int
 */
int LogPackets::packetNr() {
	return packetNumber;
}

/**
 * Returns number of probe packets
 * @return int
 */
int LogPackets::numberOfProbePackets() {
	return probePackets;
}

/**
 * Returns size of payload
 * @return int
 */
int LogPackets::payloadsize() {
	return payload;
}

/**
 * Returns true, when packet was dropped
 * @return bool
 */
bool LogPackets::boolDroppedMeasured() {
	return dropped;
}


/**
 * Set send timestamp as simtime_t
 * @param tstamp
 */
void LogPackets::setNormSendTstamp(simtime_t tstamp) {
	sendTstamp = tstamp;
}

/**
 * Set receive timestamp as simtime_t
 * @param tstamp
 */
void LogPackets::setNormRecTstamp(simtime_t tstamp) {
	recTstamp = tstamp;
}

/**
 * Set sequence number as int
 * @param nr
 */
void LogPackets::setSeqNr(int nr) {
	seqNumber = nr;
}

/**
 * Set packet number as int
 * @param nr
 */
void LogPackets::setPacketNr(int nr) {
	packetNumber = nr;
}

/**
 * Set number of probe packet as int
 * @param nr
 */
void LogPackets::setNumberOfProbePackets(int nr) {
	probePackets = nr;
}

/**
 * Set payload size as in
 * @param size
 */
void LogPackets::setPayloadsize(int size) {
	payload = size;
}

/**
 * Set true, when packet is dropped
 * @param packetDropped
 */
void LogPackets::setBoolDroppedMeasured(bool packetDropped) {
	dropped = packetDropped;
}

} /* namespace projekt */

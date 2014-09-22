//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//


// where are the files, which are not defined in this message?
cplusplus {{
    #include "Bbdelay.h"
    #include "Datarate.h"
}};
// announcing some decalarations
class noncobject projekt::Datarate;
class noncobject projekt::Bbdelay;

packet MyPacket{
	bool droppedSimulated;                 // true, when packet is dropped
	int payloadSize;                       // size of payload
	int sequenceNumber;                    // current sequence number
	int packetNumber;                      // current packet number
	int numberOfProbePackets;              // current number of probe packets
	string destinationIP;                  // IP of destination
	string sourceIP;                       // IP from source
	string sourcePort;                     // Source-Port
	string destinationPort;                // Destination-Port
	simtime_t startTimeForLogging;         // start time in first node as simtime_t
	simtime_t sendingTimeForLogging;       // start/sending time in current queue as simtime_t
	simtime_t arrivalTimeForLogging;       // arrival time in last node as simtime_t
	simtime_t earliestTimeForDelivery;     // earliest delivery time, when waiting in queue
	string timestamps[];                   // array for logging stamps
	
    bool droppedMeasured;                  // piggyback measured boolean, wheather the packet was dropped
	simtime_t recTstampMeasured;           // piggyback measured timestamp of receiving the packet
	simtime_t delayMeasured;               // piggyback measured delay
	simtime_t dCRSQ;                       // piggyback delievered to CRSQ
	simtime_t dCDSQ;                       // piggyback delievered to CDSQ
	simtime_t dCDRQ;                       // piggyback delievered to CDRQ
	simtime_t dCRRQ;                       // piggyback delievered to CRRQ
	simtime_t sCRSQ;                       // piggyback send from CRSQ
	simtime_t sCDSQ;                       // piggyback send from CDSQ
	simtime_t sCDRQ;                       // piggyback send from CDRQ
	simtime_t sCRRQ;                       // piggyback send from CRRQ
	double droprateQueue;                  // piggyback droprate
	
    Bbdelay bbdelay;                       // piggyback last bbdelay for mypacket
    Datarate datarate;                     // piggyback last datarate for mypacket
   
}
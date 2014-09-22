//
//@author Tobias Krauthoff <tobias.krauthoff@hhu.de>
//@date 22.12.2013
//Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
//

// This packet represents an event, which will start the transmission-event
// for the head of queue. ArrivalTime and SendingTime for testing-mode are
// included.
packet SendHeadAndDeletePacket{
    string deletePacketWithName;        // string witch name of MyPacket, which should be head of current queue
    simtime_t arrivalTimeForTesting;    // current arrival time, neede for logging and testing
    simtime_t sendingTimeForTesting;    // current sending time, neede for logging and testing
}
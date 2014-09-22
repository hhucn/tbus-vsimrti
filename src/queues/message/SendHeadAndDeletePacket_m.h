//
// Generated file, do not edit! Created by opp_msgc 4.4 from queues/message/SendHeadAndDeletePacket.msg.
//

#ifndef _SENDHEADANDDELETEPACKET_M_H_
#define _SENDHEADANDDELETEPACKET_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0404
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif



/**
 * Class generated from <tt>queues/message/SendHeadAndDeletePacket.msg</tt> by opp_msgc.
 * <pre>
 * packet SendHeadAndDeletePacket{
 *     string deletePacketWithName;        
 *     simtime_t arrivalTimeForTesting;    
 *     simtime_t sendingTimeForTesting;    
 * }
 * </pre>
 */
class SendHeadAndDeletePacket : public ::cPacket
{
  protected:
    opp_string deletePacketWithName_var;
    simtime_t arrivalTimeForTesting_var;
    simtime_t sendingTimeForTesting_var;

  private:
    void copy(const SendHeadAndDeletePacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const SendHeadAndDeletePacket&);

  public:
    SendHeadAndDeletePacket(const char *name=NULL, int kind=0);
    SendHeadAndDeletePacket(const SendHeadAndDeletePacket& other);
    virtual ~SendHeadAndDeletePacket();
    SendHeadAndDeletePacket& operator=(const SendHeadAndDeletePacket& other);
    virtual SendHeadAndDeletePacket *dup() const {return new SendHeadAndDeletePacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual const char * getDeletePacketWithName() const;
    virtual void setDeletePacketWithName(const char * deletePacketWithName);
    virtual simtime_t getArrivalTimeForTesting() const;
    virtual void setArrivalTimeForTesting(simtime_t arrivalTimeForTesting);
    virtual simtime_t getSendingTimeForTesting() const;
    virtual void setSendingTimeForTesting(simtime_t sendingTimeForTesting);
};

inline void doPacking(cCommBuffer *b, SendHeadAndDeletePacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, SendHeadAndDeletePacket& obj) {obj.parsimUnpack(b);}


#endif // _SENDHEADANDDELETEPACKET_M_H_

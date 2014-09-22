//
// Generated file, do not edit! Created by opp_msgc 4.4 from queues/message/MyPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "MyPacket_m.h"

USING_NAMESPACE

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(MyPacket);

MyPacket::MyPacket(const char *name, int kind) : cPacket(name,kind)
{
    this->droppedSimulated_var = 0;
    this->payloadSize_var = 0;
    this->sequenceNumber_var = 0;
    this->packetNumber_var = 0;
    this->numberOfProbePackets_var = 0;
    this->destinationIP_var = 0;
    this->sourceIP_var = 0;
    this->sourcePort_var = 0;
    this->destinationPort_var = 0;
    this->startTimeForLogging_var = 0;
    this->sendingTimeForLogging_var = 0;
    this->arrivalTimeForLogging_var = 0;
    this->earliestTimeForDelivery_var = 0;
    timestamps_arraysize = 0;
    this->timestamps_var = 0;
    this->droppedMeasured_var = 0;
    this->recTstampMeasured_var = 0;
    this->delayMeasured_var = 0;
    this->dCRSQ_var = 0;
    this->dCDSQ_var = 0;
    this->dCDRQ_var = 0;
    this->dCRRQ_var = 0;
    this->sCRSQ_var = 0;
    this->sCDSQ_var = 0;
    this->sCDRQ_var = 0;
    this->sCRRQ_var = 0;
    this->droprateQueue_var = 0;
}

MyPacket::MyPacket(const MyPacket& other) : cPacket(other)
{
    timestamps_arraysize = 0;
    this->timestamps_var = 0;
    copy(other);
}

MyPacket::~MyPacket()
{
    delete [] timestamps_var;
}

MyPacket& MyPacket::operator=(const MyPacket& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void MyPacket::copy(const MyPacket& other)
{
    this->droppedSimulated_var = other.droppedSimulated_var;
    this->payloadSize_var = other.payloadSize_var;
    this->sequenceNumber_var = other.sequenceNumber_var;
    this->packetNumber_var = other.packetNumber_var;
    this->numberOfProbePackets_var = other.numberOfProbePackets_var;
    this->destinationIP_var = other.destinationIP_var;
    this->sourceIP_var = other.sourceIP_var;
    this->sourcePort_var = other.sourcePort_var;
    this->destinationPort_var = other.destinationPort_var;
    this->startTimeForLogging_var = other.startTimeForLogging_var;
    this->sendingTimeForLogging_var = other.sendingTimeForLogging_var;
    this->arrivalTimeForLogging_var = other.arrivalTimeForLogging_var;
    this->earliestTimeForDelivery_var = other.earliestTimeForDelivery_var;
    delete [] this->timestamps_var;
    this->timestamps_var = (other.timestamps_arraysize==0) ? NULL : new opp_string[other.timestamps_arraysize];
    timestamps_arraysize = other.timestamps_arraysize;
    for (unsigned int i=0; i<timestamps_arraysize; i++)
        this->timestamps_var[i] = other.timestamps_var[i];
    this->droppedMeasured_var = other.droppedMeasured_var;
    this->recTstampMeasured_var = other.recTstampMeasured_var;
    this->delayMeasured_var = other.delayMeasured_var;
    this->dCRSQ_var = other.dCRSQ_var;
    this->dCDSQ_var = other.dCDSQ_var;
    this->dCDRQ_var = other.dCDRQ_var;
    this->dCRRQ_var = other.dCRRQ_var;
    this->sCRSQ_var = other.sCRSQ_var;
    this->sCDSQ_var = other.sCDSQ_var;
    this->sCDRQ_var = other.sCDRQ_var;
    this->sCRRQ_var = other.sCRRQ_var;
    this->droprateQueue_var = other.droprateQueue_var;
    this->bbdelay_var = other.bbdelay_var;
    this->datarate_var = other.datarate_var;
}

void MyPacket::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->droppedSimulated_var);
    doPacking(b,this->payloadSize_var);
    doPacking(b,this->sequenceNumber_var);
    doPacking(b,this->packetNumber_var);
    doPacking(b,this->numberOfProbePackets_var);
    doPacking(b,this->destinationIP_var);
    doPacking(b,this->sourceIP_var);
    doPacking(b,this->sourcePort_var);
    doPacking(b,this->destinationPort_var);
    doPacking(b,this->startTimeForLogging_var);
    doPacking(b,this->sendingTimeForLogging_var);
    doPacking(b,this->arrivalTimeForLogging_var);
    doPacking(b,this->earliestTimeForDelivery_var);
    b->pack(timestamps_arraysize);
    doPacking(b,this->timestamps_var,timestamps_arraysize);
    doPacking(b,this->droppedMeasured_var);
    doPacking(b,this->recTstampMeasured_var);
    doPacking(b,this->delayMeasured_var);
    doPacking(b,this->dCRSQ_var);
    doPacking(b,this->dCDSQ_var);
    doPacking(b,this->dCDRQ_var);
    doPacking(b,this->dCRRQ_var);
    doPacking(b,this->sCRSQ_var);
    doPacking(b,this->sCDSQ_var);
    doPacking(b,this->sCDRQ_var);
    doPacking(b,this->sCRRQ_var);
    doPacking(b,this->droprateQueue_var);
    doPacking(b,this->bbdelay_var);
    doPacking(b,this->datarate_var);
}

void MyPacket::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->droppedSimulated_var);
    doUnpacking(b,this->payloadSize_var);
    doUnpacking(b,this->sequenceNumber_var);
    doUnpacking(b,this->packetNumber_var);
    doUnpacking(b,this->numberOfProbePackets_var);
    doUnpacking(b,this->destinationIP_var);
    doUnpacking(b,this->sourceIP_var);
    doUnpacking(b,this->sourcePort_var);
    doUnpacking(b,this->destinationPort_var);
    doUnpacking(b,this->startTimeForLogging_var);
    doUnpacking(b,this->sendingTimeForLogging_var);
    doUnpacking(b,this->arrivalTimeForLogging_var);
    doUnpacking(b,this->earliestTimeForDelivery_var);
    delete [] this->timestamps_var;
    b->unpack(timestamps_arraysize);
    if (timestamps_arraysize==0) {
        this->timestamps_var = 0;
    } else {
        this->timestamps_var = new opp_string[timestamps_arraysize];
        doUnpacking(b,this->timestamps_var,timestamps_arraysize);
    }
    doUnpacking(b,this->droppedMeasured_var);
    doUnpacking(b,this->recTstampMeasured_var);
    doUnpacking(b,this->delayMeasured_var);
    doUnpacking(b,this->dCRSQ_var);
    doUnpacking(b,this->dCDSQ_var);
    doUnpacking(b,this->dCDRQ_var);
    doUnpacking(b,this->dCRRQ_var);
    doUnpacking(b,this->sCRSQ_var);
    doUnpacking(b,this->sCDSQ_var);
    doUnpacking(b,this->sCDRQ_var);
    doUnpacking(b,this->sCRRQ_var);
    doUnpacking(b,this->droprateQueue_var);
    doUnpacking(b,this->bbdelay_var);
    doUnpacking(b,this->datarate_var);
}

bool MyPacket::getDroppedSimulated() const
{
    return droppedSimulated_var;
}

void MyPacket::setDroppedSimulated(bool droppedSimulated)
{
    this->droppedSimulated_var = droppedSimulated;
}

int MyPacket::getPayloadSize() const
{
    return payloadSize_var;
}

void MyPacket::setPayloadSize(int payloadSize)
{
    this->payloadSize_var = payloadSize;
}

int MyPacket::getSequenceNumber() const
{
    return sequenceNumber_var;
}

void MyPacket::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber_var = sequenceNumber;
}

int MyPacket::getPacketNumber() const
{
    return packetNumber_var;
}

void MyPacket::setPacketNumber(int packetNumber)
{
    this->packetNumber_var = packetNumber;
}

int MyPacket::getNumberOfProbePackets() const
{
    return numberOfProbePackets_var;
}

void MyPacket::setNumberOfProbePackets(int numberOfProbePackets)
{
    this->numberOfProbePackets_var = numberOfProbePackets;
}

const char * MyPacket::getDestinationIP() const
{
    return destinationIP_var.c_str();
}

void MyPacket::setDestinationIP(const char * destinationIP)
{
    this->destinationIP_var = destinationIP;
}

const char * MyPacket::getSourceIP() const
{
    return sourceIP_var.c_str();
}

void MyPacket::setSourceIP(const char * sourceIP)
{
    this->sourceIP_var = sourceIP;
}

const char * MyPacket::getSourcePort() const
{
    return sourcePort_var.c_str();
}

void MyPacket::setSourcePort(const char * sourcePort)
{
    this->sourcePort_var = sourcePort;
}

const char * MyPacket::getDestinationPort() const
{
    return destinationPort_var.c_str();
}

void MyPacket::setDestinationPort(const char * destinationPort)
{
    this->destinationPort_var = destinationPort;
}

simtime_t MyPacket::getStartTimeForLogging() const
{
    return startTimeForLogging_var;
}

void MyPacket::setStartTimeForLogging(simtime_t startTimeForLogging)
{
    this->startTimeForLogging_var = startTimeForLogging;
}

simtime_t MyPacket::getSendingTimeForLogging() const
{
    return sendingTimeForLogging_var;
}

void MyPacket::setSendingTimeForLogging(simtime_t sendingTimeForLogging)
{
    this->sendingTimeForLogging_var = sendingTimeForLogging;
}

simtime_t MyPacket::getArrivalTimeForLogging() const
{
    return arrivalTimeForLogging_var;
}

void MyPacket::setArrivalTimeForLogging(simtime_t arrivalTimeForLogging)
{
    this->arrivalTimeForLogging_var = arrivalTimeForLogging;
}

simtime_t MyPacket::getEarliestTimeForDelivery() const
{
    return earliestTimeForDelivery_var;
}

void MyPacket::setEarliestTimeForDelivery(simtime_t earliestTimeForDelivery)
{
    this->earliestTimeForDelivery_var = earliestTimeForDelivery;
}

void MyPacket::setTimestampsArraySize(unsigned int size)
{
    opp_string *timestamps_var2 = (size==0) ? NULL : new opp_string[size];
    unsigned int sz = timestamps_arraysize < size ? timestamps_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        timestamps_var2[i] = this->timestamps_var[i];
    for (unsigned int i=sz; i<size; i++)
        timestamps_var2[i] = 0;
    timestamps_arraysize = size;
    delete [] this->timestamps_var;
    this->timestamps_var = timestamps_var2;
}

unsigned int MyPacket::getTimestampsArraySize() const
{
    return timestamps_arraysize;
}

const char * MyPacket::getTimestamps(unsigned int k) const
{
    if (k>=timestamps_arraysize) throw cRuntimeError("Array of size %d indexed by %d", timestamps_arraysize, k);
    return timestamps_var[k].c_str();
}

void MyPacket::setTimestamps(unsigned int k, const char * timestamps)
{
    if (k>=timestamps_arraysize) throw cRuntimeError("Array of size %d indexed by %d", timestamps_arraysize, k);
    this->timestamps_var[k] = timestamps;
}

bool MyPacket::getDroppedMeasured() const
{
    return droppedMeasured_var;
}

void MyPacket::setDroppedMeasured(bool droppedMeasured)
{
    this->droppedMeasured_var = droppedMeasured;
}

simtime_t MyPacket::getRecTstampMeasured() const
{
    return recTstampMeasured_var;
}

void MyPacket::setRecTstampMeasured(simtime_t recTstampMeasured)
{
    this->recTstampMeasured_var = recTstampMeasured;
}

simtime_t MyPacket::getDelayMeasured() const
{
    return delayMeasured_var;
}

void MyPacket::setDelayMeasured(simtime_t delayMeasured)
{
    this->delayMeasured_var = delayMeasured;
}

simtime_t MyPacket::getDCRSQ() const
{
    return dCRSQ_var;
}

void MyPacket::setDCRSQ(simtime_t dCRSQ)
{
    this->dCRSQ_var = dCRSQ;
}

simtime_t MyPacket::getDCDSQ() const
{
    return dCDSQ_var;
}

void MyPacket::setDCDSQ(simtime_t dCDSQ)
{
    this->dCDSQ_var = dCDSQ;
}

simtime_t MyPacket::getDCDRQ() const
{
    return dCDRQ_var;
}

void MyPacket::setDCDRQ(simtime_t dCDRQ)
{
    this->dCDRQ_var = dCDRQ;
}

simtime_t MyPacket::getDCRRQ() const
{
    return dCRRQ_var;
}

void MyPacket::setDCRRQ(simtime_t dCRRQ)
{
    this->dCRRQ_var = dCRRQ;
}

simtime_t MyPacket::getSCRSQ() const
{
    return sCRSQ_var;
}

void MyPacket::setSCRSQ(simtime_t sCRSQ)
{
    this->sCRSQ_var = sCRSQ;
}

simtime_t MyPacket::getSCDSQ() const
{
    return sCDSQ_var;
}

void MyPacket::setSCDSQ(simtime_t sCDSQ)
{
    this->sCDSQ_var = sCDSQ;
}

simtime_t MyPacket::getSCDRQ() const
{
    return sCDRQ_var;
}

void MyPacket::setSCDRQ(simtime_t sCDRQ)
{
    this->sCDRQ_var = sCDRQ;
}

simtime_t MyPacket::getSCRRQ() const
{
    return sCRRQ_var;
}

void MyPacket::setSCRRQ(simtime_t sCRRQ)
{
    this->sCRRQ_var = sCRRQ;
}

double MyPacket::getDroprateQueue() const
{
    return droprateQueue_var;
}

void MyPacket::setDroprateQueue(double droprateQueue)
{
    this->droprateQueue_var = droprateQueue;
}

projekt::Bbdelay& MyPacket::getBbdelay()
{
    return bbdelay_var;
}

void MyPacket::setBbdelay(const projekt::Bbdelay& bbdelay)
{
    this->bbdelay_var = bbdelay;
}

projekt::Datarate& MyPacket::getDatarate()
{
    return datarate_var;
}

void MyPacket::setDatarate(const projekt::Datarate& datarate)
{
    this->datarate_var = datarate;
}

class MyPacketDescriptor : public cClassDescriptor
{
  public:
    MyPacketDescriptor();
    virtual ~MyPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MyPacketDescriptor);

MyPacketDescriptor::MyPacketDescriptor() : cClassDescriptor("MyPacket", "cPacket")
{
}

MyPacketDescriptor::~MyPacketDescriptor()
{
}

bool MyPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MyPacket *>(obj)!=NULL;
}

const char *MyPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MyPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 28+basedesc->getFieldCount(object) : 28;
}

unsigned int MyPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<28) ? fieldTypeFlags[field] : 0;
}

const char *MyPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "droppedSimulated",
        "payloadSize",
        "sequenceNumber",
        "packetNumber",
        "numberOfProbePackets",
        "destinationIP",
        "sourceIP",
        "sourcePort",
        "destinationPort",
        "startTimeForLogging",
        "sendingTimeForLogging",
        "arrivalTimeForLogging",
        "earliestTimeForDelivery",
        "timestamps",
        "droppedMeasured",
        "recTstampMeasured",
        "delayMeasured",
        "dCRSQ",
        "dCDSQ",
        "dCDRQ",
        "dCRRQ",
        "sCRSQ",
        "sCDSQ",
        "sCDRQ",
        "sCRRQ",
        "droprateQueue",
        "bbdelay",
        "datarate",
    };
    return (field>=0 && field<28) ? fieldNames[field] : NULL;
}

int MyPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "droppedSimulated")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "payloadSize")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumber")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetNumber")==0) return base+3;
    if (fieldName[0]=='n' && strcmp(fieldName, "numberOfProbePackets")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationIP")==0) return base+5;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceIP")==0) return base+6;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourcePort")==0) return base+7;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationPort")==0) return base+8;
    if (fieldName[0]=='s' && strcmp(fieldName, "startTimeForLogging")==0) return base+9;
    if (fieldName[0]=='s' && strcmp(fieldName, "sendingTimeForLogging")==0) return base+10;
    if (fieldName[0]=='a' && strcmp(fieldName, "arrivalTimeForLogging")==0) return base+11;
    if (fieldName[0]=='e' && strcmp(fieldName, "earliestTimeForDelivery")==0) return base+12;
    if (fieldName[0]=='t' && strcmp(fieldName, "timestamps")==0) return base+13;
    if (fieldName[0]=='d' && strcmp(fieldName, "droppedMeasured")==0) return base+14;
    if (fieldName[0]=='r' && strcmp(fieldName, "recTstampMeasured")==0) return base+15;
    if (fieldName[0]=='d' && strcmp(fieldName, "delayMeasured")==0) return base+16;
    if (fieldName[0]=='d' && strcmp(fieldName, "dCRSQ")==0) return base+17;
    if (fieldName[0]=='d' && strcmp(fieldName, "dCDSQ")==0) return base+18;
    if (fieldName[0]=='d' && strcmp(fieldName, "dCDRQ")==0) return base+19;
    if (fieldName[0]=='d' && strcmp(fieldName, "dCRRQ")==0) return base+20;
    if (fieldName[0]=='s' && strcmp(fieldName, "sCRSQ")==0) return base+21;
    if (fieldName[0]=='s' && strcmp(fieldName, "sCDSQ")==0) return base+22;
    if (fieldName[0]=='s' && strcmp(fieldName, "sCDRQ")==0) return base+23;
    if (fieldName[0]=='s' && strcmp(fieldName, "sCRRQ")==0) return base+24;
    if (fieldName[0]=='d' && strcmp(fieldName, "droprateQueue")==0) return base+25;
    if (fieldName[0]=='b' && strcmp(fieldName, "bbdelay")==0) return base+26;
    if (fieldName[0]=='d' && strcmp(fieldName, "datarate")==0) return base+27;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MyPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "int",
        "int",
        "int",
        "int",
        "string",
        "string",
        "string",
        "string",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "string",
        "bool",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "double",
        "Bbdelay",
        "Datarate",
    };
    return (field>=0 && field<28) ? fieldTypeStrings[field] : NULL;
}

const char *MyPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MyPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MyPacket *pp = (MyPacket *)object; (void)pp;
    switch (field) {
        case 13: return pp->getTimestampsArraySize();
        default: return 0;
    }
}

std::string MyPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MyPacket *pp = (MyPacket *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getDroppedSimulated());
        case 1: return long2string(pp->getPayloadSize());
        case 2: return long2string(pp->getSequenceNumber());
        case 3: return long2string(pp->getPacketNumber());
        case 4: return long2string(pp->getNumberOfProbePackets());
        case 5: return oppstring2string(pp->getDestinationIP());
        case 6: return oppstring2string(pp->getSourceIP());
        case 7: return oppstring2string(pp->getSourcePort());
        case 8: return oppstring2string(pp->getDestinationPort());
        case 9: return double2string(pp->getStartTimeForLogging());
        case 10: return double2string(pp->getSendingTimeForLogging());
        case 11: return double2string(pp->getArrivalTimeForLogging());
        case 12: return double2string(pp->getEarliestTimeForDelivery());
        case 13: return oppstring2string(pp->getTimestamps(i));
        case 14: return bool2string(pp->getDroppedMeasured());
        case 15: return double2string(pp->getRecTstampMeasured());
        case 16: return double2string(pp->getDelayMeasured());
        case 17: return double2string(pp->getDCRSQ());
        case 18: return double2string(pp->getDCDSQ());
        case 19: return double2string(pp->getDCDRQ());
        case 20: return double2string(pp->getDCRRQ());
        case 21: return double2string(pp->getSCRSQ());
        case 22: return double2string(pp->getSCDSQ());
        case 23: return double2string(pp->getSCDRQ());
        case 24: return double2string(pp->getSCRRQ());
        case 25: return double2string(pp->getDroprateQueue());
        case 26: {std::stringstream out; out << pp->getBbdelay(); return out.str();}
        case 27: {std::stringstream out; out << pp->getDatarate(); return out.str();}
        default: return "";
    }
}

bool MyPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MyPacket *pp = (MyPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setDroppedSimulated(string2bool(value)); return true;
        case 1: pp->setPayloadSize(string2long(value)); return true;
        case 2: pp->setSequenceNumber(string2long(value)); return true;
        case 3: pp->setPacketNumber(string2long(value)); return true;
        case 4: pp->setNumberOfProbePackets(string2long(value)); return true;
        case 5: pp->setDestinationIP((value)); return true;
        case 6: pp->setSourceIP((value)); return true;
        case 7: pp->setSourcePort((value)); return true;
        case 8: pp->setDestinationPort((value)); return true;
        case 9: pp->setStartTimeForLogging(string2double(value)); return true;
        case 10: pp->setSendingTimeForLogging(string2double(value)); return true;
        case 11: pp->setArrivalTimeForLogging(string2double(value)); return true;
        case 12: pp->setEarliestTimeForDelivery(string2double(value)); return true;
        case 13: pp->setTimestamps(i,(value)); return true;
        case 14: pp->setDroppedMeasured(string2bool(value)); return true;
        case 15: pp->setRecTstampMeasured(string2double(value)); return true;
        case 16: pp->setDelayMeasured(string2double(value)); return true;
        case 17: pp->setDCRSQ(string2double(value)); return true;
        case 18: pp->setDCDSQ(string2double(value)); return true;
        case 19: pp->setDCDRQ(string2double(value)); return true;
        case 20: pp->setDCRRQ(string2double(value)); return true;
        case 21: pp->setSCRSQ(string2double(value)); return true;
        case 22: pp->setSCDSQ(string2double(value)); return true;
        case 23: pp->setSCDRQ(string2double(value)); return true;
        case 24: pp->setSCRRQ(string2double(value)); return true;
        case 25: pp->setDroprateQueue(string2double(value)); return true;
        default: return false;
    }
}

const char *MyPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        "projekt::Bbdelay",
        "projekt::Datarate",
    };
    return (field>=0 && field<28) ? fieldStructNames[field] : NULL;
}

void *MyPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MyPacket *pp = (MyPacket *)object; (void)pp;
    switch (field) {
        case 26: return (void *)(&pp->getBbdelay()); break;
        case 27: return (void *)(&pp->getDatarate()); break;
        default: return NULL;
    }
}



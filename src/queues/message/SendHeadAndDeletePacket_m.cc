//
// Generated file, do not edit! Created by opp_msgc 4.4 from queues/message/SendHeadAndDeletePacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SendHeadAndDeletePacket_m.h"

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




Register_Class(SendHeadAndDeletePacket);

SendHeadAndDeletePacket::SendHeadAndDeletePacket(const char *name, int kind) : cPacket(name,kind)
{
    this->deletePacketWithName_var = 0;
    this->arrivalTimeForTesting_var = 0;
    this->sendingTimeForTesting_var = 0;
}

SendHeadAndDeletePacket::SendHeadAndDeletePacket(const SendHeadAndDeletePacket& other) : cPacket(other)
{
    copy(other);
}

SendHeadAndDeletePacket::~SendHeadAndDeletePacket()
{
}

SendHeadAndDeletePacket& SendHeadAndDeletePacket::operator=(const SendHeadAndDeletePacket& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void SendHeadAndDeletePacket::copy(const SendHeadAndDeletePacket& other)
{
    this->deletePacketWithName_var = other.deletePacketWithName_var;
    this->arrivalTimeForTesting_var = other.arrivalTimeForTesting_var;
    this->sendingTimeForTesting_var = other.sendingTimeForTesting_var;
}

void SendHeadAndDeletePacket::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->deletePacketWithName_var);
    doPacking(b,this->arrivalTimeForTesting_var);
    doPacking(b,this->sendingTimeForTesting_var);
}

void SendHeadAndDeletePacket::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->deletePacketWithName_var);
    doUnpacking(b,this->arrivalTimeForTesting_var);
    doUnpacking(b,this->sendingTimeForTesting_var);
}

const char * SendHeadAndDeletePacket::getDeletePacketWithName() const
{
    return deletePacketWithName_var.c_str();
}

void SendHeadAndDeletePacket::setDeletePacketWithName(const char * deletePacketWithName)
{
    this->deletePacketWithName_var = deletePacketWithName;
}

simtime_t SendHeadAndDeletePacket::getArrivalTimeForTesting() const
{
    return arrivalTimeForTesting_var;
}

void SendHeadAndDeletePacket::setArrivalTimeForTesting(simtime_t arrivalTimeForTesting)
{
    this->arrivalTimeForTesting_var = arrivalTimeForTesting;
}

simtime_t SendHeadAndDeletePacket::getSendingTimeForTesting() const
{
    return sendingTimeForTesting_var;
}

void SendHeadAndDeletePacket::setSendingTimeForTesting(simtime_t sendingTimeForTesting)
{
    this->sendingTimeForTesting_var = sendingTimeForTesting;
}

class SendHeadAndDeletePacketDescriptor : public cClassDescriptor
{
  public:
    SendHeadAndDeletePacketDescriptor();
    virtual ~SendHeadAndDeletePacketDescriptor();

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

Register_ClassDescriptor(SendHeadAndDeletePacketDescriptor);

SendHeadAndDeletePacketDescriptor::SendHeadAndDeletePacketDescriptor() : cClassDescriptor("SendHeadAndDeletePacket", "cPacket")
{
}

SendHeadAndDeletePacketDescriptor::~SendHeadAndDeletePacketDescriptor()
{
}

bool SendHeadAndDeletePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SendHeadAndDeletePacket *>(obj)!=NULL;
}

const char *SendHeadAndDeletePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SendHeadAndDeletePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int SendHeadAndDeletePacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *SendHeadAndDeletePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "deletePacketWithName",
        "arrivalTimeForTesting",
        "sendingTimeForTesting",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int SendHeadAndDeletePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "deletePacketWithName")==0) return base+0;
    if (fieldName[0]=='a' && strcmp(fieldName, "arrivalTimeForTesting")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "sendingTimeForTesting")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SendHeadAndDeletePacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "simtime_t",
        "simtime_t",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *SendHeadAndDeletePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SendHeadAndDeletePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SendHeadAndDeletePacket *pp = (SendHeadAndDeletePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SendHeadAndDeletePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SendHeadAndDeletePacket *pp = (SendHeadAndDeletePacket *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getDeletePacketWithName());
        case 1: return double2string(pp->getArrivalTimeForTesting());
        case 2: return double2string(pp->getSendingTimeForTesting());
        default: return "";
    }
}

bool SendHeadAndDeletePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SendHeadAndDeletePacket *pp = (SendHeadAndDeletePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setDeletePacketWithName((value)); return true;
        case 1: pp->setArrivalTimeForTesting(string2double(value)); return true;
        case 2: pp->setSendingTimeForTesting(string2double(value)); return true;
        default: return false;
    }
}

const char *SendHeadAndDeletePacketDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *SendHeadAndDeletePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SendHeadAndDeletePacket *pp = (SendHeadAndDeletePacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



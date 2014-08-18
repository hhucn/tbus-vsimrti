//
// Generated file, do not edit! Created by opp_msgc 4.4 from linklayer/message/TbusAirFrame.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "TbusAirFrame_m.h"

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




Register_Class(TbusAirFrame);

TbusAirFrame::TbusAirFrame(const char *name, int kind) : cPacket(name,kind)
{
    this->clientMessage_var = 0;
    this->hostName_var = 0;
}

TbusAirFrame::TbusAirFrame(const TbusAirFrame& other) : cPacket(other)
{
    copy(other);
}

TbusAirFrame::~TbusAirFrame()
{
}

TbusAirFrame& TbusAirFrame::operator=(const TbusAirFrame& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void TbusAirFrame::copy(const TbusAirFrame& other)
{
    this->clientMessage_var = other.clientMessage_var;
    this->hostName_var = other.hostName_var;
}

void TbusAirFrame::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->clientMessage_var);
    doPacking(b,this->hostName_var);
}

void TbusAirFrame::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->clientMessage_var);
    doUnpacking(b,this->hostName_var);
}

bool TbusAirFrame::getClientMessage() const
{
    return clientMessage_var;
}

void TbusAirFrame::setClientMessage(bool clientMessage)
{
    this->clientMessage_var = clientMessage;
}

const char * TbusAirFrame::getHostName() const
{
    return hostName_var.c_str();
}

void TbusAirFrame::setHostName(const char * hostName)
{
    this->hostName_var = hostName;
}

class TbusAirFrameDescriptor : public cClassDescriptor
{
  public:
    TbusAirFrameDescriptor();
    virtual ~TbusAirFrameDescriptor();

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

Register_ClassDescriptor(TbusAirFrameDescriptor);

TbusAirFrameDescriptor::TbusAirFrameDescriptor() : cClassDescriptor("TbusAirFrame", "cPacket")
{
}

TbusAirFrameDescriptor::~TbusAirFrameDescriptor()
{
}

bool TbusAirFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<TbusAirFrame *>(obj)!=NULL;
}

const char *TbusAirFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int TbusAirFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int TbusAirFrameDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *TbusAirFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "clientMessage",
        "hostName",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int TbusAirFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "clientMessage")==0) return base+0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hostName")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *TbusAirFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "string",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *TbusAirFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int TbusAirFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    TbusAirFrame *pp = (TbusAirFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string TbusAirFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    TbusAirFrame *pp = (TbusAirFrame *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getClientMessage());
        case 1: return oppstring2string(pp->getHostName());
        default: return "";
    }
}

bool TbusAirFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    TbusAirFrame *pp = (TbusAirFrame *)object; (void)pp;
    switch (field) {
        case 0: pp->setClientMessage(string2bool(value)); return true;
        case 1: pp->setHostName((value)); return true;
        default: return false;
    }
}

const char *TbusAirFrameDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *TbusAirFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    TbusAirFrame *pp = (TbusAirFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



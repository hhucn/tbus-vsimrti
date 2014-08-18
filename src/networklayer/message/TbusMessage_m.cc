//
// Generated file, do not edit! Created by opp_msgc 4.4 from networklayer/message/TbusMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "TbusMessage_m.h"

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




Register_Class(TbusMessage);

TbusMessage::TbusMessage(const char *name, int kind) : cPacket(name,kind)
{
    this->encMessage_var = 0;
    this->senderHostName_var = 0;
    this->receiverHostName_var = 0;
}

TbusMessage::TbusMessage(const TbusMessage& other) : cPacket(other)
{
    copy(other);
}

TbusMessage::~TbusMessage()
{
}

TbusMessage& TbusMessage::operator=(const TbusMessage& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void TbusMessage::copy(const TbusMessage& other)
{
    this->encMessage_var = other.encMessage_var;
    this->senderHostName_var = other.senderHostName_var;
    this->receiverHostName_var = other.receiverHostName_var;
}

void TbusMessage::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->encMessage_var);
    doPacking(b,this->senderHostName_var);
    doPacking(b,this->receiverHostName_var);
}

void TbusMessage::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->encMessage_var);
    doUnpacking(b,this->senderHostName_var);
    doUnpacking(b,this->receiverHostName_var);
}

const char * TbusMessage::getEncMessage() const
{
    return encMessage_var.c_str();
}

void TbusMessage::setEncMessage(const char * encMessage)
{
    this->encMessage_var = encMessage;
}

const char * TbusMessage::getSenderHostName() const
{
    return senderHostName_var.c_str();
}

void TbusMessage::setSenderHostName(const char * senderHostName)
{
    this->senderHostName_var = senderHostName;
}

const char * TbusMessage::getReceiverHostName() const
{
    return receiverHostName_var.c_str();
}

void TbusMessage::setReceiverHostName(const char * receiverHostName)
{
    this->receiverHostName_var = receiverHostName;
}

class TbusMessageDescriptor : public cClassDescriptor
{
  public:
    TbusMessageDescriptor();
    virtual ~TbusMessageDescriptor();

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

Register_ClassDescriptor(TbusMessageDescriptor);

TbusMessageDescriptor::TbusMessageDescriptor() : cClassDescriptor("TbusMessage", "cPacket")
{
}

TbusMessageDescriptor::~TbusMessageDescriptor()
{
}

bool TbusMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<TbusMessage *>(obj)!=NULL;
}

const char *TbusMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int TbusMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int TbusMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *TbusMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "encMessage",
        "senderHostName",
        "receiverHostName",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int TbusMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='e' && strcmp(fieldName, "encMessage")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderHostName")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "receiverHostName")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *TbusMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
        "string",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *TbusMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int TbusMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    TbusMessage *pp = (TbusMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string TbusMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    TbusMessage *pp = (TbusMessage *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getEncMessage());
        case 1: return oppstring2string(pp->getSenderHostName());
        case 2: return oppstring2string(pp->getReceiverHostName());
        default: return "";
    }
}

bool TbusMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    TbusMessage *pp = (TbusMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setEncMessage((value)); return true;
        case 1: pp->setSenderHostName((value)); return true;
        case 2: pp->setReceiverHostName((value)); return true;
        default: return false;
    }
}

const char *TbusMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *TbusMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    TbusMessage *pp = (TbusMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



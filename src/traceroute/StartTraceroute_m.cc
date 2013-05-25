//
// Generated file, do not edit! Created by opp_msgc 4.2 from traceroute/StartTraceroute.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "StartTraceroute_m.h"

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




StartTraceroute::StartTraceroute() : ICMPMessage()
{
    this->port_var = 0;
}

StartTraceroute::StartTraceroute(const StartTraceroute& other) : ICMPMessage(other)
{
    copy(other);
}

StartTraceroute::~StartTraceroute()
{
}

StartTraceroute& StartTraceroute::operator=(const StartTraceroute& other)
{
    if (this==&other) return *this;
    ICMPMessage::operator=(other);
    copy(other);
    return *this;
}

void StartTraceroute::copy(const StartTraceroute& other)
{
    this->Dest_var = other.Dest_var;
    this->port_var = other.port_var;
}

void StartTraceroute::parsimPack(cCommBuffer *b)
{
    doPacking(b,(ICMPMessage&)*this);
    doPacking(b,this->Dest_var);
    doPacking(b,this->port_var);
}

void StartTraceroute::parsimUnpack(cCommBuffer *b)
{
    doUnpacking(b,(ICMPMessage&)*this);
    doUnpacking(b,this->Dest_var);
    doUnpacking(b,this->port_var);
}

IPvXAddress& StartTraceroute::getDest()
{
    return Dest_var;
}

void StartTraceroute::setDest(const IPvXAddress& Dest)
{
    this->Dest_var = Dest;
}

int StartTraceroute::getPort() const
{
    return port_var;
}

void StartTraceroute::setPort(int port)
{
    this->port_var = port;
}

class StartTracerouteDescriptor : public cClassDescriptor
{
  public:
    StartTracerouteDescriptor();
    virtual ~StartTracerouteDescriptor();

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

Register_ClassDescriptor(StartTracerouteDescriptor);

StartTracerouteDescriptor::StartTracerouteDescriptor() : cClassDescriptor("StartTraceroute", "ICMPMessage")
{
}

StartTracerouteDescriptor::~StartTracerouteDescriptor()
{
}

bool StartTracerouteDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<StartTraceroute *>(obj)!=NULL;
}

const char *StartTracerouteDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int StartTracerouteDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int StartTracerouteDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *StartTracerouteDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "Dest",
        "port",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int StartTracerouteDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='D' && strcmp(fieldName, "Dest")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "port")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *StartTracerouteDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "IPvXAddress",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *StartTracerouteDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int StartTracerouteDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    StartTraceroute *pp = (StartTraceroute *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string StartTracerouteDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    StartTraceroute *pp = (StartTraceroute *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getDest(); return out.str();}
        case 1: return long2string(pp->getPort());
        default: return "";
    }
}

bool StartTracerouteDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    StartTraceroute *pp = (StartTraceroute *)object; (void)pp;
    switch (field) {
        case 1: pp->setPort(string2long(value)); return true;
        default: return false;
    }
}

const char *StartTracerouteDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "IPvXAddress",
        NULL,
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *StartTracerouteDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    StartTraceroute *pp = (StartTraceroute *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getDest()); break;
        default: return NULL;
    }
}



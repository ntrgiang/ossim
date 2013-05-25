//
// Generated file, do not edit! Created by opp_msgc 4.2 from traceroute/RouteMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "RouteMsg_m.h"

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




Register_Class(RouteMsg);

RouteMsg::RouteMsg(const char *name, int kind) : cPacket(name,kind)
{
}

RouteMsg::RouteMsg(const RouteMsg& other) : cPacket(other)
{
    copy(other);
}

RouteMsg::~RouteMsg()
{
}

RouteMsg& RouteMsg::operator=(const RouteMsg& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void RouteMsg::copy(const RouteMsg& other)
{
    this->route_var = other.route_var;
    this->Dest_var = other.Dest_var;
}

void RouteMsg::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->route_var);
    doPacking(b,this->Dest_var);
}

void RouteMsg::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->route_var);
    doUnpacking(b,this->Dest_var);
}

TracerouteTable::Route& RouteMsg::getRoute()
{
    return route_var;
}

void RouteMsg::setRoute(const TracerouteTable::Route& route)
{
    this->route_var = route;
}

IPvXAddress& RouteMsg::getDest()
{
    return Dest_var;
}

void RouteMsg::setDest(const IPvXAddress& Dest)
{
    this->Dest_var = Dest;
}

class RouteMsgDescriptor : public cClassDescriptor
{
  public:
    RouteMsgDescriptor();
    virtual ~RouteMsgDescriptor();

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

Register_ClassDescriptor(RouteMsgDescriptor);

RouteMsgDescriptor::RouteMsgDescriptor() : cClassDescriptor("RouteMsg", "cPacket")
{
}

RouteMsgDescriptor::~RouteMsgDescriptor()
{
}

bool RouteMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<RouteMsg *>(obj)!=NULL;
}

const char *RouteMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int RouteMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int RouteMsgDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *RouteMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "route",
        "Dest",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int RouteMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "route")==0) return base+0;
    if (fieldName[0]=='D' && strcmp(fieldName, "Dest")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *RouteMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "TracerouteTable::Route",
        "IPvXAddress",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *RouteMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int RouteMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    RouteMsg *pp = (RouteMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string RouteMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    RouteMsg *pp = (RouteMsg *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getRoute(); return out.str();}
        case 1: {std::stringstream out; out << pp->getDest(); return out.str();}
        default: return "";
    }
}

bool RouteMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    RouteMsg *pp = (RouteMsg *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *RouteMsgDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "TracerouteTable::Route",
        "IPvXAddress",
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *RouteMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    RouteMsg *pp = (RouteMsg *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getRoute()); break;
        case 1: return (void *)(&pp->getDest()); break;
        default: return NULL;
    }
}



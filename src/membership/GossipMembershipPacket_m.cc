//
// Generated file, do not edit! Created by opp_msgc 4.2 from applications/common/messages/GossipMembershipPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "GossipMembershipPacket_m.h"

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




EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("GossipMembershipPacketType");
    if (!e) enums.getInstance()->add(e = new cEnum("GossipMembershipPacketType"));
    e->insert(GOSSIP_SUBSCRIPTION_PT, "GOSSIP_SUBSCRIPTION_PT");
    e->insert(GOSSIP_SUBS_ACK_PT, "GOSSIP_SUBS_ACK_PT");
    e->insert(GOSSIP_UNSUBS_FROM_PVIEW_PT, "GOSSIP_UNSUBS_FROM_PVIEW_PT");
    e->insert(GOSSIP_UNSUBS_FROM_IVIEW_PT, "GOSSIP_UNSUBS_FROM_IVIEW_PT");
    e->insert(GOSSIP_HEARTBEAT_PT, "GOSSIP_HEARTBEAT_PT");
    e->insert(GOSSIP_APP_PT, "GOSSIP_APP_PT");
);

Register_Class(GossipMembershipPacket);

GossipMembershipPacket::GossipMembershipPacket(const char *name, int kind) : PeerStreamingPacket(name,kind)
{
    this->setPacketGroup(PACKET_GROUP_GOSSIP_OVERLAY);

    this->packetType_var = 0;
}

GossipMembershipPacket::GossipMembershipPacket(const GossipMembershipPacket& other) : PeerStreamingPacket(other)
{
    copy(other);
}

GossipMembershipPacket::~GossipMembershipPacket()
{
}

GossipMembershipPacket& GossipMembershipPacket::operator=(const GossipMembershipPacket& other)
{
    if (this==&other) return *this;
    PeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void GossipMembershipPacket::copy(const GossipMembershipPacket& other)
{
    this->packetType_var = other.packetType_var;
}

void GossipMembershipPacket::parsimPack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
}

void GossipMembershipPacket::parsimUnpack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
}

short GossipMembershipPacket::getPacketType() const
{
    return packetType_var;
}

void GossipMembershipPacket::setPacketType(short packetType)
{
    this->packetType_var = packetType;
}

class GossipMembershipPacketDescriptor : public cClassDescriptor
{
  public:
    GossipMembershipPacketDescriptor();
    virtual ~GossipMembershipPacketDescriptor();

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

Register_ClassDescriptor(GossipMembershipPacketDescriptor);

GossipMembershipPacketDescriptor::GossipMembershipPacketDescriptor() : cClassDescriptor("GossipMembershipPacket", "PeerStreamingPacket")
{
}

GossipMembershipPacketDescriptor::~GossipMembershipPacketDescriptor()
{
}

bool GossipMembershipPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<GossipMembershipPacket *>(obj)!=NULL;
}

const char *GossipMembershipPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int GossipMembershipPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int GossipMembershipPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *GossipMembershipPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetType",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int GossipMembershipPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *GossipMembershipPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "short",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *GossipMembershipPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"getter")) return "getPacketType";
            if (!strcmp(propertyname,"enum")) return "GossipMembershipPacketType";
            if (!strcmp(propertyname,"setter")) return "setPacketType";
            return NULL;
        default: return NULL;
    }
}

int GossipMembershipPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    GossipMembershipPacket *pp = (GossipMembershipPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string GossipMembershipPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    GossipMembershipPacket *pp = (GossipMembershipPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPacketType());
        default: return "";
    }
}

bool GossipMembershipPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    GossipMembershipPacket *pp = (GossipMembershipPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2long(value)); return true;
        default: return false;
    }
}

const char *GossipMembershipPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *GossipMembershipPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    GossipMembershipPacket *pp = (GossipMembershipPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(GossipSubscriptionPacket);

GossipSubscriptionPacket::GossipSubscriptionPacket(const char *name, int kind) : GossipMembershipPacket(name,kind)
{
    this->setPacketType(GOSSIP_SUBSCRIPTION_PT);

    this->fwdSubscription_var = 0;
    this->subscriberAddress_var = 0;
    this->subscriberPort_var = 0;
}

GossipSubscriptionPacket::GossipSubscriptionPacket(const GossipSubscriptionPacket& other) : GossipMembershipPacket(other)
{
    copy(other);
}

GossipSubscriptionPacket::~GossipSubscriptionPacket()
{
}

GossipSubscriptionPacket& GossipSubscriptionPacket::operator=(const GossipSubscriptionPacket& other)
{
    if (this==&other) return *this;
    GossipMembershipPacket::operator=(other);
    copy(other);
    return *this;
}

void GossipSubscriptionPacket::copy(const GossipSubscriptionPacket& other)
{
    this->fwdSubscription_var = other.fwdSubscription_var;
    this->subscriberAddress_var = other.subscriberAddress_var;
    this->subscriberPort_var = other.subscriberPort_var;
}

void GossipSubscriptionPacket::parsimPack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimPack(b);
    doPacking(b,this->fwdSubscription_var);
    doPacking(b,this->subscriberAddress_var);
    doPacking(b,this->subscriberPort_var);
}

void GossipSubscriptionPacket::parsimUnpack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimUnpack(b);
    doUnpacking(b,this->fwdSubscription_var);
    doUnpacking(b,this->subscriberAddress_var);
    doUnpacking(b,this->subscriberPort_var);
}

bool GossipSubscriptionPacket::isFwdSubscription() const
{
    return fwdSubscription_var;
}

void GossipSubscriptionPacket::setFwdSubscription(bool fwdSubscription)
{
    this->fwdSubscription_var = fwdSubscription;
}

const char * GossipSubscriptionPacket::getSubscriberAddress() const
{
    return subscriberAddress_var.c_str();
}

void GossipSubscriptionPacket::setSubscriberAddress(const char * subscriberAddress)
{
    this->subscriberAddress_var = subscriberAddress;
}

int GossipSubscriptionPacket::getSubscriberPort() const
{
    return subscriberPort_var;
}

void GossipSubscriptionPacket::setSubscriberPort(int subscriberPort)
{
    this->subscriberPort_var = subscriberPort;
}

class GossipSubscriptionPacketDescriptor : public cClassDescriptor
{
  public:
    GossipSubscriptionPacketDescriptor();
    virtual ~GossipSubscriptionPacketDescriptor();

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

Register_ClassDescriptor(GossipSubscriptionPacketDescriptor);

GossipSubscriptionPacketDescriptor::GossipSubscriptionPacketDescriptor() : cClassDescriptor("GossipSubscriptionPacket", "GossipMembershipPacket")
{
}

GossipSubscriptionPacketDescriptor::~GossipSubscriptionPacketDescriptor()
{
}

bool GossipSubscriptionPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<GossipSubscriptionPacket *>(obj)!=NULL;
}

const char *GossipSubscriptionPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int GossipSubscriptionPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int GossipSubscriptionPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *GossipSubscriptionPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "fwdSubscription",
        "subscriberAddress",
        "subscriberPort",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int GossipSubscriptionPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fwdSubscription")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "subscriberAddress")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "subscriberPort")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *GossipSubscriptionPacketDescriptor::getFieldTypeString(void *object, int field) const
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
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *GossipSubscriptionPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"getter")) return "isFwdSubscription";
            if (!strcmp(propertyname,"setter")) return "setFwdSubscription";
            return NULL;
        case 1:
            if (!strcmp(propertyname,"getter")) return "getSubscriberAddress";
            if (!strcmp(propertyname,"setter")) return "setSubscriberAddress";
            return NULL;
        case 2:
            if (!strcmp(propertyname,"getter")) return "getSubscriberPort";
            if (!strcmp(propertyname,"setter")) return "setSubscriberPort";
            return NULL;
        default: return NULL;
    }
}

int GossipSubscriptionPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    GossipSubscriptionPacket *pp = (GossipSubscriptionPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string GossipSubscriptionPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    GossipSubscriptionPacket *pp = (GossipSubscriptionPacket *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->isFwdSubscription());
        case 1: return oppstring2string(pp->getSubscriberAddress());
        case 2: return long2string(pp->getSubscriberPort());
        default: return "";
    }
}

bool GossipSubscriptionPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    GossipSubscriptionPacket *pp = (GossipSubscriptionPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setFwdSubscription(string2bool(value)); return true;
        case 1: pp->setSubscriberAddress((value)); return true;
        case 2: pp->setSubscriberPort(string2long(value)); return true;
        default: return false;
    }
}

const char *GossipSubscriptionPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *GossipSubscriptionPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    GossipSubscriptionPacket *pp = (GossipSubscriptionPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(GossipSubsAckPacket);

GossipSubsAckPacket::GossipSubsAckPacket(const char *name, int kind) : GossipMembershipPacket(name,kind)
{
    this->setPacketType(GOSSIP_SUBS_ACK_PT);
}

GossipSubsAckPacket::GossipSubsAckPacket(const GossipSubsAckPacket& other) : GossipMembershipPacket(other)
{
    copy(other);
}

GossipSubsAckPacket::~GossipSubsAckPacket()
{
}

GossipSubsAckPacket& GossipSubsAckPacket::operator=(const GossipSubsAckPacket& other)
{
    if (this==&other) return *this;
    GossipMembershipPacket::operator=(other);
    copy(other);
    return *this;
}

void GossipSubsAckPacket::copy(const GossipSubsAckPacket& other)
{
}

void GossipSubsAckPacket::parsimPack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimPack(b);
}

void GossipSubsAckPacket::parsimUnpack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimUnpack(b);
}

class GossipSubsAckPacketDescriptor : public cClassDescriptor
{
  public:
    GossipSubsAckPacketDescriptor();
    virtual ~GossipSubsAckPacketDescriptor();

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

Register_ClassDescriptor(GossipSubsAckPacketDescriptor);

GossipSubsAckPacketDescriptor::GossipSubsAckPacketDescriptor() : cClassDescriptor("GossipSubsAckPacket", "GossipMembershipPacket")
{
}

GossipSubsAckPacketDescriptor::~GossipSubsAckPacketDescriptor()
{
}

bool GossipSubsAckPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<GossipSubsAckPacket *>(obj)!=NULL;
}

const char *GossipSubsAckPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int GossipSubsAckPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int GossipSubsAckPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *GossipSubsAckPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int GossipSubsAckPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *GossipSubsAckPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *GossipSubsAckPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int GossipSubsAckPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    GossipSubsAckPacket *pp = (GossipSubsAckPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string GossipSubsAckPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    GossipSubsAckPacket *pp = (GossipSubsAckPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool GossipSubsAckPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    GossipSubsAckPacket *pp = (GossipSubsAckPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *GossipSubsAckPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *GossipSubsAckPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    GossipSubsAckPacket *pp = (GossipSubsAckPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(GossipUnSubsFromPartialViewPacket);

GossipUnSubsFromPartialViewPacket::GossipUnSubsFromPartialViewPacket(const char *name, int kind) : GossipMembershipPacket(name,kind)
{
    this->setPacketType(GOSSIP_UNSUBS_FROM_PVIEW_PT);
}

GossipUnSubsFromPartialViewPacket::GossipUnSubsFromPartialViewPacket(const GossipUnSubsFromPartialViewPacket& other) : GossipMembershipPacket(other)
{
    copy(other);
}

GossipUnSubsFromPartialViewPacket::~GossipUnSubsFromPartialViewPacket()
{
}

GossipUnSubsFromPartialViewPacket& GossipUnSubsFromPartialViewPacket::operator=(const GossipUnSubsFromPartialViewPacket& other)
{
    if (this==&other) return *this;
    GossipMembershipPacket::operator=(other);
    copy(other);
    return *this;
}

void GossipUnSubsFromPartialViewPacket::copy(const GossipUnSubsFromPartialViewPacket& other)
{
}

void GossipUnSubsFromPartialViewPacket::parsimPack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimPack(b);
}

void GossipUnSubsFromPartialViewPacket::parsimUnpack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimUnpack(b);
}

class GossipUnSubsFromPartialViewPacketDescriptor : public cClassDescriptor
{
  public:
    GossipUnSubsFromPartialViewPacketDescriptor();
    virtual ~GossipUnSubsFromPartialViewPacketDescriptor();

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

Register_ClassDescriptor(GossipUnSubsFromPartialViewPacketDescriptor);

GossipUnSubsFromPartialViewPacketDescriptor::GossipUnSubsFromPartialViewPacketDescriptor() : cClassDescriptor("GossipUnSubsFromPartialViewPacket", "GossipMembershipPacket")
{
}

GossipUnSubsFromPartialViewPacketDescriptor::~GossipUnSubsFromPartialViewPacketDescriptor()
{
}

bool GossipUnSubsFromPartialViewPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<GossipUnSubsFromPartialViewPacket *>(obj)!=NULL;
}

const char *GossipUnSubsFromPartialViewPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int GossipUnSubsFromPartialViewPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int GossipUnSubsFromPartialViewPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *GossipUnSubsFromPartialViewPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int GossipUnSubsFromPartialViewPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *GossipUnSubsFromPartialViewPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *GossipUnSubsFromPartialViewPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int GossipUnSubsFromPartialViewPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    GossipUnSubsFromPartialViewPacket *pp = (GossipUnSubsFromPartialViewPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string GossipUnSubsFromPartialViewPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    GossipUnSubsFromPartialViewPacket *pp = (GossipUnSubsFromPartialViewPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool GossipUnSubsFromPartialViewPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    GossipUnSubsFromPartialViewPacket *pp = (GossipUnSubsFromPartialViewPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *GossipUnSubsFromPartialViewPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *GossipUnSubsFromPartialViewPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    GossipUnSubsFromPartialViewPacket *pp = (GossipUnSubsFromPartialViewPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(GossipUnSubsFromInViewPacket);

GossipUnSubsFromInViewPacket::GossipUnSubsFromInViewPacket(const char *name, int kind) : GossipMembershipPacket(name,kind)
{
    this->setPacketType(GOSSIP_UNSUBS_FROM_IVIEW_PT);
}

GossipUnSubsFromInViewPacket::GossipUnSubsFromInViewPacket(const GossipUnSubsFromInViewPacket& other) : GossipMembershipPacket(other)
{
    copy(other);
}

GossipUnSubsFromInViewPacket::~GossipUnSubsFromInViewPacket()
{
}

GossipUnSubsFromInViewPacket& GossipUnSubsFromInViewPacket::operator=(const GossipUnSubsFromInViewPacket& other)
{
    if (this==&other) return *this;
    GossipMembershipPacket::operator=(other);
    copy(other);
    return *this;
}

void GossipUnSubsFromInViewPacket::copy(const GossipUnSubsFromInViewPacket& other)
{
}

void GossipUnSubsFromInViewPacket::parsimPack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimPack(b);
}

void GossipUnSubsFromInViewPacket::parsimUnpack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimUnpack(b);
}

class GossipUnSubsFromInViewPacketDescriptor : public cClassDescriptor
{
  public:
    GossipUnSubsFromInViewPacketDescriptor();
    virtual ~GossipUnSubsFromInViewPacketDescriptor();

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

Register_ClassDescriptor(GossipUnSubsFromInViewPacketDescriptor);

GossipUnSubsFromInViewPacketDescriptor::GossipUnSubsFromInViewPacketDescriptor() : cClassDescriptor("GossipUnSubsFromInViewPacket", "GossipMembershipPacket")
{
}

GossipUnSubsFromInViewPacketDescriptor::~GossipUnSubsFromInViewPacketDescriptor()
{
}

bool GossipUnSubsFromInViewPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<GossipUnSubsFromInViewPacket *>(obj)!=NULL;
}

const char *GossipUnSubsFromInViewPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int GossipUnSubsFromInViewPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int GossipUnSubsFromInViewPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *GossipUnSubsFromInViewPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int GossipUnSubsFromInViewPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *GossipUnSubsFromInViewPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *GossipUnSubsFromInViewPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int GossipUnSubsFromInViewPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    GossipUnSubsFromInViewPacket *pp = (GossipUnSubsFromInViewPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string GossipUnSubsFromInViewPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    GossipUnSubsFromInViewPacket *pp = (GossipUnSubsFromInViewPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool GossipUnSubsFromInViewPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    GossipUnSubsFromInViewPacket *pp = (GossipUnSubsFromInViewPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *GossipUnSubsFromInViewPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *GossipUnSubsFromInViewPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    GossipUnSubsFromInViewPacket *pp = (GossipUnSubsFromInViewPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(GossipHeartbeatPacket);

GossipHeartbeatPacket::GossipHeartbeatPacket(const char *name, int kind) : GossipMembershipPacket(name,kind)
{
    this->setPacketType(GOSSIP_HEARTBEAT_PT);
}

GossipHeartbeatPacket::GossipHeartbeatPacket(const GossipHeartbeatPacket& other) : GossipMembershipPacket(other)
{
    copy(other);
}

GossipHeartbeatPacket::~GossipHeartbeatPacket()
{
}

GossipHeartbeatPacket& GossipHeartbeatPacket::operator=(const GossipHeartbeatPacket& other)
{
    if (this==&other) return *this;
    GossipMembershipPacket::operator=(other);
    copy(other);
    return *this;
}

void GossipHeartbeatPacket::copy(const GossipHeartbeatPacket& other)
{
}

void GossipHeartbeatPacket::parsimPack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimPack(b);
}

void GossipHeartbeatPacket::parsimUnpack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimUnpack(b);
}

class GossipHeartbeatPacketDescriptor : public cClassDescriptor
{
  public:
    GossipHeartbeatPacketDescriptor();
    virtual ~GossipHeartbeatPacketDescriptor();

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

Register_ClassDescriptor(GossipHeartbeatPacketDescriptor);

GossipHeartbeatPacketDescriptor::GossipHeartbeatPacketDescriptor() : cClassDescriptor("GossipHeartbeatPacket", "GossipMembershipPacket")
{
}

GossipHeartbeatPacketDescriptor::~GossipHeartbeatPacketDescriptor()
{
}

bool GossipHeartbeatPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<GossipHeartbeatPacket *>(obj)!=NULL;
}

const char *GossipHeartbeatPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int GossipHeartbeatPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int GossipHeartbeatPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *GossipHeartbeatPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int GossipHeartbeatPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *GossipHeartbeatPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *GossipHeartbeatPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int GossipHeartbeatPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    GossipHeartbeatPacket *pp = (GossipHeartbeatPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string GossipHeartbeatPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    GossipHeartbeatPacket *pp = (GossipHeartbeatPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool GossipHeartbeatPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    GossipHeartbeatPacket *pp = (GossipHeartbeatPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *GossipHeartbeatPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *GossipHeartbeatPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    GossipHeartbeatPacket *pp = (GossipHeartbeatPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(GossipApplicationPacket);

GossipApplicationPacket::GossipApplicationPacket(const char *name, int kind) : GossipMembershipPacket(name,kind)
{
    this->setPacketType(GOSSIP_APP_PT);

    this->messageId_var = 0;
    this->timeStamp_var = 0;
}

GossipApplicationPacket::GossipApplicationPacket(const GossipApplicationPacket& other) : GossipMembershipPacket(other)
{
    copy(other);
}

GossipApplicationPacket::~GossipApplicationPacket()
{
}

GossipApplicationPacket& GossipApplicationPacket::operator=(const GossipApplicationPacket& other)
{
    if (this==&other) return *this;
    GossipMembershipPacket::operator=(other);
    copy(other);
    return *this;
}

void GossipApplicationPacket::copy(const GossipApplicationPacket& other)
{
    this->rootAddress_var = other.rootAddress_var;
    this->messageId_var = other.messageId_var;
    this->timeStamp_var = other.timeStamp_var;
}

void GossipApplicationPacket::parsimPack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimPack(b);
    doPacking(b,this->rootAddress_var);
    doPacking(b,this->messageId_var);
    doPacking(b,this->timeStamp_var);
}

void GossipApplicationPacket::parsimUnpack(cCommBuffer *b)
{
    GossipMembershipPacket::parsimUnpack(b);
    doUnpacking(b,this->rootAddress_var);
    doUnpacking(b,this->messageId_var);
    doUnpacking(b,this->timeStamp_var);
}

IPvXAddress& GossipApplicationPacket::getRootAddress()
{
    return rootAddress_var;
}

void GossipApplicationPacket::setRootAddress(const IPvXAddress& rootAddress)
{
    this->rootAddress_var = rootAddress;
}

long GossipApplicationPacket::getMessageId() const
{
    return messageId_var;
}

void GossipApplicationPacket::setMessageId(long messageId)
{
    this->messageId_var = messageId;
}

double GossipApplicationPacket::getTimeStamp() const
{
    return timeStamp_var;
}

void GossipApplicationPacket::setTimeStamp(double timeStamp)
{
    this->timeStamp_var = timeStamp;
}

class GossipApplicationPacketDescriptor : public cClassDescriptor
{
  public:
    GossipApplicationPacketDescriptor();
    virtual ~GossipApplicationPacketDescriptor();

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

Register_ClassDescriptor(GossipApplicationPacketDescriptor);

GossipApplicationPacketDescriptor::GossipApplicationPacketDescriptor() : cClassDescriptor("GossipApplicationPacket", "GossipMembershipPacket")
{
}

GossipApplicationPacketDescriptor::~GossipApplicationPacketDescriptor()
{
}

bool GossipApplicationPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<GossipApplicationPacket *>(obj)!=NULL;
}

const char *GossipApplicationPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int GossipApplicationPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int GossipApplicationPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *GossipApplicationPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "rootAddress",
        "messageId",
        "timeStamp",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int GossipApplicationPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "rootAddress")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "messageId")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeStamp")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *GossipApplicationPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "IPvXAddress",
        "long",
        "double",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *GossipApplicationPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"getter")) return "getRootAddress";
            if (!strcmp(propertyname,"setter")) return "setRootAddress";
            return NULL;
        case 1:
            if (!strcmp(propertyname,"getter")) return "getMessageId";
            if (!strcmp(propertyname,"setter")) return "setMessageId";
            return NULL;
        case 2:
            if (!strcmp(propertyname,"getter")) return "getTimeStamp";
            if (!strcmp(propertyname,"setter")) return "setTimeStamp";
            return NULL;
        default: return NULL;
    }
}

int GossipApplicationPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    GossipApplicationPacket *pp = (GossipApplicationPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string GossipApplicationPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    GossipApplicationPacket *pp = (GossipApplicationPacket *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getRootAddress(); return out.str();}
        case 1: return long2string(pp->getMessageId());
        case 2: return double2string(pp->getTimeStamp());
        default: return "";
    }
}

bool GossipApplicationPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    GossipApplicationPacket *pp = (GossipApplicationPacket *)object; (void)pp;
    switch (field) {
        case 1: pp->setMessageId(string2long(value)); return true;
        case 2: pp->setTimeStamp(string2double(value)); return true;
        default: return false;
    }
}

const char *GossipApplicationPacketDescriptor::getFieldStructName(void *object, int field) const
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
        NULL,
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *GossipApplicationPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    GossipApplicationPacket *pp = (GossipApplicationPacket *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getRootAddress()); break;
        default: return NULL;
    }
}



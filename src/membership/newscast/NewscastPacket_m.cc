//
// Generated file, do not edit! Created by opp_msgc 4.2 from membership/newscast/NewscastPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "NewscastPacket_m.h"

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
    cEnum *e = cEnum::find("NewscastPacketType");
    if (!e) enums.getInstance()->add(e = new cEnum("NewscastPacketType"));
    e->insert(NEWSCAST_REQUEST, "NEWSCAST_REQUEST");
    e->insert(NEWSCAST_REPLY, "NEWSCAST_REPLY");
);

Register_Class(NewscastPacket);

NewscastPacket::NewscastPacket(const char *name, int kind) : PeerStreamingPacket(name,kind)
{
    this->setPacketGroup(PACKET_GROUP_GOSSIP_OVERLAY);

    this->packetType_var = 0;
    take(&(this->cache_var));
}

NewscastPacket::NewscastPacket(const NewscastPacket& other) : PeerStreamingPacket(other)
{
    take(&(this->cache_var));
    copy(other);
}

NewscastPacket::~NewscastPacket()
{
    drop(&(this->cache_var));
}

NewscastPacket& NewscastPacket::operator=(const NewscastPacket& other)
{
    if (this==&other) return *this;
    PeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void NewscastPacket::copy(const NewscastPacket& other)
{
    this->packetType_var = other.packetType_var;
    this->cache_var = other.cache_var;
    this->cache_var.setName(other.cache_var.getName());
}

void NewscastPacket::parsimPack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
    doPacking(b,this->cache_var);
}

void NewscastPacket::parsimUnpack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->cache_var);
}

short NewscastPacket::getPacketType() const
{
    return packetType_var;
}

void NewscastPacket::setPacketType(short packetType)
{
    this->packetType_var = packetType;
}

NewscastCache& NewscastPacket::getCache()
{
    return cache_var;
}

void NewscastPacket::setCache(const NewscastCache& cache)
{
    this->cache_var = cache;
}

class NewscastPacketDescriptor : public cClassDescriptor
{
  public:
    NewscastPacketDescriptor();
    virtual ~NewscastPacketDescriptor();

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

Register_ClassDescriptor(NewscastPacketDescriptor);

NewscastPacketDescriptor::NewscastPacketDescriptor() : cClassDescriptor("NewscastPacket", "PeerStreamingPacket")
{
}

NewscastPacketDescriptor::~NewscastPacketDescriptor()
{
}

bool NewscastPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NewscastPacket *>(obj)!=NULL;
}

const char *NewscastPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NewscastPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int NewscastPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *NewscastPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetType",
        "cache",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int NewscastPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "cache")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NewscastPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "short",
        "NewscastCache",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *NewscastPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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
            if (!strcmp(propertyname,"enum")) return "NewscastPacketType";
            if (!strcmp(propertyname,"setter")) return "setPacketType";
            return NULL;
        case 1:
            if (!strcmp(propertyname,"getter")) return "getCache";
            if (!strcmp(propertyname,"setter")) return "setCache";
            return NULL;
        default: return NULL;
    }
}

int NewscastPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NewscastPacket *pp = (NewscastPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NewscastPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NewscastPacket *pp = (NewscastPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPacketType());
        case 1: {std::stringstream out; out << pp->getCache(); return out.str();}
        default: return "";
    }
}

bool NewscastPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NewscastPacket *pp = (NewscastPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2long(value)); return true;
        default: return false;
    }
}

const char *NewscastPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        "NewscastCache",
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *NewscastPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NewscastPacket *pp = (NewscastPacket *)object; (void)pp;
    switch (field) {
        case 1: return (void *)static_cast<cObject *>(&pp->getCache()); break;
        default: return NULL;
    }
}

Register_Class(NewscastRequestPacket);

NewscastRequestPacket::NewscastRequestPacket(const char *name, int kind) : NewscastPacket(name,kind)
{
    this->setPacketType(NEWSCAST_REQUEST);
}

NewscastRequestPacket::NewscastRequestPacket(const NewscastRequestPacket& other) : NewscastPacket(other)
{
    copy(other);
}

NewscastRequestPacket::~NewscastRequestPacket()
{
}

NewscastRequestPacket& NewscastRequestPacket::operator=(const NewscastRequestPacket& other)
{
    if (this==&other) return *this;
    NewscastPacket::operator=(other);
    copy(other);
    return *this;
}

void NewscastRequestPacket::copy(const NewscastRequestPacket& other)
{
}

void NewscastRequestPacket::parsimPack(cCommBuffer *b)
{
    NewscastPacket::parsimPack(b);
}

void NewscastRequestPacket::parsimUnpack(cCommBuffer *b)
{
    NewscastPacket::parsimUnpack(b);
}

class NewscastRequestPacketDescriptor : public cClassDescriptor
{
  public:
    NewscastRequestPacketDescriptor();
    virtual ~NewscastRequestPacketDescriptor();

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

Register_ClassDescriptor(NewscastRequestPacketDescriptor);

NewscastRequestPacketDescriptor::NewscastRequestPacketDescriptor() : cClassDescriptor("NewscastRequestPacket", "NewscastPacket")
{
}

NewscastRequestPacketDescriptor::~NewscastRequestPacketDescriptor()
{
}

bool NewscastRequestPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NewscastRequestPacket *>(obj)!=NULL;
}

const char *NewscastRequestPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NewscastRequestPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int NewscastRequestPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *NewscastRequestPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int NewscastRequestPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NewscastRequestPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *NewscastRequestPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int NewscastRequestPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NewscastRequestPacket *pp = (NewscastRequestPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NewscastRequestPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NewscastRequestPacket *pp = (NewscastRequestPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool NewscastRequestPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NewscastRequestPacket *pp = (NewscastRequestPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *NewscastRequestPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *NewscastRequestPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NewscastRequestPacket *pp = (NewscastRequestPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(NewscastReplyPacket);

NewscastReplyPacket::NewscastReplyPacket(const char *name, int kind) : NewscastPacket(name,kind)
{
    this->setPacketType(NEWSCAST_REPLY);
}

NewscastReplyPacket::NewscastReplyPacket(const NewscastReplyPacket& other) : NewscastPacket(other)
{
    copy(other);
}

NewscastReplyPacket::~NewscastReplyPacket()
{
}

NewscastReplyPacket& NewscastReplyPacket::operator=(const NewscastReplyPacket& other)
{
    if (this==&other) return *this;
    NewscastPacket::operator=(other);
    copy(other);
    return *this;
}

void NewscastReplyPacket::copy(const NewscastReplyPacket& other)
{
}

void NewscastReplyPacket::parsimPack(cCommBuffer *b)
{
    NewscastPacket::parsimPack(b);
}

void NewscastReplyPacket::parsimUnpack(cCommBuffer *b)
{
    NewscastPacket::parsimUnpack(b);
}

class NewscastReplyPacketDescriptor : public cClassDescriptor
{
  public:
    NewscastReplyPacketDescriptor();
    virtual ~NewscastReplyPacketDescriptor();

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

Register_ClassDescriptor(NewscastReplyPacketDescriptor);

NewscastReplyPacketDescriptor::NewscastReplyPacketDescriptor() : cClassDescriptor("NewscastReplyPacket", "NewscastPacket")
{
}

NewscastReplyPacketDescriptor::~NewscastReplyPacketDescriptor()
{
}

bool NewscastReplyPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NewscastReplyPacket *>(obj)!=NULL;
}

const char *NewscastReplyPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NewscastReplyPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int NewscastReplyPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *NewscastReplyPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int NewscastReplyPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NewscastReplyPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *NewscastReplyPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int NewscastReplyPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NewscastReplyPacket *pp = (NewscastReplyPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NewscastReplyPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NewscastReplyPacket *pp = (NewscastReplyPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool NewscastReplyPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NewscastReplyPacket *pp = (NewscastReplyPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *NewscastReplyPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *NewscastReplyPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NewscastReplyPacket *pp = (NewscastReplyPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



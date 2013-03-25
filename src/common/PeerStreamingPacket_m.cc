//
// Generated file, do not edit! Created by opp_msgc 4.2 from common/PeerStreamingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "PeerStreamingPacket_m.h"

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
    cEnum *e = cEnum::find("StreamingPacketGroup");
    if (!e) enums.getInstance()->add(e = new cEnum("StreamingPacketGroup"));
    e->insert(PACKET_GROUP_GOSSIP_OVERLAY, "PACKET_GROUP_GOSSIP_OVERLAY");
    e->insert(PACKET_GROUP_MESH_OVERLAY, "PACKET_GROUP_MESH_OVERLAY");
    e->insert(PACKET_GROUP_VIDEO_CHUNK, "PACKET_GROUP_VIDEO_CHUNK");
    e->insert(PACKET_GROUP_TREE_OVERLAY, "PACKET_GROUP_TREE_OVERLAY");
);

Register_Class(PeerStreamingPacket);

PeerStreamingPacket::PeerStreamingPacket(const char *name, int kind) : cPacket(name,kind)
{
    this->packetGroup_var = 0;
}

PeerStreamingPacket::PeerStreamingPacket(const PeerStreamingPacket& other) : cPacket(other)
{
    copy(other);
}

PeerStreamingPacket::~PeerStreamingPacket()
{
}

PeerStreamingPacket& PeerStreamingPacket::operator=(const PeerStreamingPacket& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void PeerStreamingPacket::copy(const PeerStreamingPacket& other)
{
    this->packetGroup_var = other.packetGroup_var;
}

void PeerStreamingPacket::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->packetGroup_var);
}

void PeerStreamingPacket::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->packetGroup_var);
}

unsigned short PeerStreamingPacket::getPacketGroup() const
{
    return packetGroup_var;
}

void PeerStreamingPacket::setPacketGroup(unsigned short packetGroup)
{
    this->packetGroup_var = packetGroup;
}

class PeerStreamingPacketDescriptor : public cClassDescriptor
{
  public:
    PeerStreamingPacketDescriptor();
    virtual ~PeerStreamingPacketDescriptor();

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

Register_ClassDescriptor(PeerStreamingPacketDescriptor);

PeerStreamingPacketDescriptor::PeerStreamingPacketDescriptor() : cClassDescriptor("PeerStreamingPacket", "cPacket")
{
}

PeerStreamingPacketDescriptor::~PeerStreamingPacketDescriptor()
{
}

bool PeerStreamingPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PeerStreamingPacket *>(obj)!=NULL;
}

const char *PeerStreamingPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PeerStreamingPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int PeerStreamingPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *PeerStreamingPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetGroup",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int PeerStreamingPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetGroup")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PeerStreamingPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned short",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *PeerStreamingPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "StreamingPacketGroup";
            return NULL;
        default: return NULL;
    }
}

int PeerStreamingPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PeerStreamingPacket *pp = (PeerStreamingPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PeerStreamingPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PeerStreamingPacket *pp = (PeerStreamingPacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPacketGroup());
        default: return "";
    }
}

bool PeerStreamingPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PeerStreamingPacket *pp = (PeerStreamingPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketGroup(string2ulong(value)); return true;
        default: return false;
    }
}

const char *PeerStreamingPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *PeerStreamingPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PeerStreamingPacket *pp = (PeerStreamingPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



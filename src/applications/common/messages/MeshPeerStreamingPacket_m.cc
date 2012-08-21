//
// Generated file, do not edit! Created by opp_msgc 4.2 from applications/common/messages/MeshPeerStreamingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "MeshPeerStreamingPacket_m.h"

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
    cEnum *e = cEnum::find("MeshPeerStreamingPacketType");
    if (!e) enums.getInstance()->add(e = new cEnum("MeshPeerStreamingPacketType"));
    e->insert(MESH_PARTNERSHIP, "MESH_PARTNERSHIP");
    e->insert(MESH_KEEP_ALIVE, "MESH_KEEP_ALIVE");
    e->insert(MESH_BUFFER_MAP, "MESH_BUFFER_MAP");
    e->insert(MESH_CHUNK_REQUEST, "MESH_CHUNK_REQUEST");
    e->insert(MESH_CHUNK_REPLY, "MESH_CHUNK_REPLY");
    e->insert(MESH_VIDEO_CHUNK, "MESH_VIDEO_CHUNK");
    e->insert(MESH_PARTNERSHIP_REQUEST, "MESH_PARTNERSHIP_REQUEST");
    e->insert(MESH_PARTNERSHIP_ACCEPT, "MESH_PARTNERSHIP_ACCEPT");
    e->insert(MESH_PARTNERSHIP_REJECT, "MESH_PARTNERSHIP_REJECT");
);

Register_Class(MeshPeerStreamingPacket);

MeshPeerStreamingPacket::MeshPeerStreamingPacket(const char *name, int kind) : PeerStreamingPacket(name,kind)
{
    this->setPacketGroup(PACKET_GROUP_MESH_OVERLAY);

    this->packetType_var = 0;
}

MeshPeerStreamingPacket::MeshPeerStreamingPacket(const MeshPeerStreamingPacket& other) : PeerStreamingPacket(other)
{
    copy(other);
}

MeshPeerStreamingPacket::~MeshPeerStreamingPacket()
{
}

MeshPeerStreamingPacket& MeshPeerStreamingPacket::operator=(const MeshPeerStreamingPacket& other)
{
    if (this==&other) return *this;
    PeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void MeshPeerStreamingPacket::copy(const MeshPeerStreamingPacket& other)
{
    this->packetType_var = other.packetType_var;
}

void MeshPeerStreamingPacket::parsimPack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
}

void MeshPeerStreamingPacket::parsimUnpack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
}

short MeshPeerStreamingPacket::getPacketType() const
{
    return packetType_var;
}

void MeshPeerStreamingPacket::setPacketType(short packetType)
{
    this->packetType_var = packetType;
}

class MeshPeerStreamingPacketDescriptor : public cClassDescriptor
{
  public:
    MeshPeerStreamingPacketDescriptor();
    virtual ~MeshPeerStreamingPacketDescriptor();

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

Register_ClassDescriptor(MeshPeerStreamingPacketDescriptor);

MeshPeerStreamingPacketDescriptor::MeshPeerStreamingPacketDescriptor() : cClassDescriptor("MeshPeerStreamingPacket", "PeerStreamingPacket")
{
}

MeshPeerStreamingPacketDescriptor::~MeshPeerStreamingPacketDescriptor()
{
}

bool MeshPeerStreamingPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MeshPeerStreamingPacket *>(obj)!=NULL;
}

const char *MeshPeerStreamingPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MeshPeerStreamingPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int MeshPeerStreamingPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *MeshPeerStreamingPacketDescriptor::getFieldName(void *object, int field) const
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

int MeshPeerStreamingPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MeshPeerStreamingPacketDescriptor::getFieldTypeString(void *object, int field) const
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

const char *MeshPeerStreamingPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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
            if (!strcmp(propertyname,"enum")) return "MeshPeerStreamingPacketType";
            if (!strcmp(propertyname,"setter")) return "setPacketType";
            return NULL;
        default: return NULL;
    }
}

int MeshPeerStreamingPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MeshPeerStreamingPacket *pp = (MeshPeerStreamingPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MeshPeerStreamingPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MeshPeerStreamingPacket *pp = (MeshPeerStreamingPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPacketType());
        default: return "";
    }
}

bool MeshPeerStreamingPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MeshPeerStreamingPacket *pp = (MeshPeerStreamingPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2long(value)); return true;
        default: return false;
    }
}

const char *MeshPeerStreamingPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *MeshPeerStreamingPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MeshPeerStreamingPacket *pp = (MeshPeerStreamingPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MeshPartnershipRequestPacket);

MeshPartnershipRequestPacket::MeshPartnershipRequestPacket(const char *name, int kind) : MeshPeerStreamingPacket(name,kind)
{
    this->setPacketType(MESH_PARTNERSHIP_REQUEST);

    this->upBw_var = 0;
}

MeshPartnershipRequestPacket::MeshPartnershipRequestPacket(const MeshPartnershipRequestPacket& other) : MeshPeerStreamingPacket(other)
{
    copy(other);
}

MeshPartnershipRequestPacket::~MeshPartnershipRequestPacket()
{
}

MeshPartnershipRequestPacket& MeshPartnershipRequestPacket::operator=(const MeshPartnershipRequestPacket& other)
{
    if (this==&other) return *this;
    MeshPeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void MeshPartnershipRequestPacket::copy(const MeshPartnershipRequestPacket& other)
{
    this->upBw_var = other.upBw_var;
}

void MeshPartnershipRequestPacket::parsimPack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimPack(b);
    doPacking(b,this->upBw_var);
}

void MeshPartnershipRequestPacket::parsimUnpack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->upBw_var);
}

double MeshPartnershipRequestPacket::getUpBw() const
{
    return upBw_var;
}

void MeshPartnershipRequestPacket::setUpBw(double upBw)
{
    this->upBw_var = upBw;
}

class MeshPartnershipRequestPacketDescriptor : public cClassDescriptor
{
  public:
    MeshPartnershipRequestPacketDescriptor();
    virtual ~MeshPartnershipRequestPacketDescriptor();

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

Register_ClassDescriptor(MeshPartnershipRequestPacketDescriptor);

MeshPartnershipRequestPacketDescriptor::MeshPartnershipRequestPacketDescriptor() : cClassDescriptor("MeshPartnershipRequestPacket", "MeshPeerStreamingPacket")
{
}

MeshPartnershipRequestPacketDescriptor::~MeshPartnershipRequestPacketDescriptor()
{
}

bool MeshPartnershipRequestPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MeshPartnershipRequestPacket *>(obj)!=NULL;
}

const char *MeshPartnershipRequestPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MeshPartnershipRequestPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int MeshPartnershipRequestPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *MeshPartnershipRequestPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "upBw",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int MeshPartnershipRequestPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='u' && strcmp(fieldName, "upBw")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MeshPartnershipRequestPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *MeshPartnershipRequestPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"default")) return "-1.0";
            return NULL;
        default: return NULL;
    }
}

int MeshPartnershipRequestPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipRequestPacket *pp = (MeshPartnershipRequestPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MeshPartnershipRequestPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipRequestPacket *pp = (MeshPartnershipRequestPacket *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getUpBw());
        default: return "";
    }
}

bool MeshPartnershipRequestPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipRequestPacket *pp = (MeshPartnershipRequestPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setUpBw(string2double(value)); return true;
        default: return false;
    }
}

const char *MeshPartnershipRequestPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *MeshPartnershipRequestPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipRequestPacket *pp = (MeshPartnershipRequestPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MeshPartnershipAcceptPacket);

MeshPartnershipAcceptPacket::MeshPartnershipAcceptPacket(const char *name, int kind) : MeshPeerStreamingPacket(name,kind)
{
    this->setPacketType(MESH_PARTNERSHIP_ACCEPT);

    this->upBw_var = 0;
}

MeshPartnershipAcceptPacket::MeshPartnershipAcceptPacket(const MeshPartnershipAcceptPacket& other) : MeshPeerStreamingPacket(other)
{
    copy(other);
}

MeshPartnershipAcceptPacket::~MeshPartnershipAcceptPacket()
{
}

MeshPartnershipAcceptPacket& MeshPartnershipAcceptPacket::operator=(const MeshPartnershipAcceptPacket& other)
{
    if (this==&other) return *this;
    MeshPeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void MeshPartnershipAcceptPacket::copy(const MeshPartnershipAcceptPacket& other)
{
    this->upBw_var = other.upBw_var;
}

void MeshPartnershipAcceptPacket::parsimPack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimPack(b);
    doPacking(b,this->upBw_var);
}

void MeshPartnershipAcceptPacket::parsimUnpack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->upBw_var);
}

double MeshPartnershipAcceptPacket::getUpBw() const
{
    return upBw_var;
}

void MeshPartnershipAcceptPacket::setUpBw(double upBw)
{
    this->upBw_var = upBw;
}

class MeshPartnershipAcceptPacketDescriptor : public cClassDescriptor
{
  public:
    MeshPartnershipAcceptPacketDescriptor();
    virtual ~MeshPartnershipAcceptPacketDescriptor();

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

Register_ClassDescriptor(MeshPartnershipAcceptPacketDescriptor);

MeshPartnershipAcceptPacketDescriptor::MeshPartnershipAcceptPacketDescriptor() : cClassDescriptor("MeshPartnershipAcceptPacket", "MeshPeerStreamingPacket")
{
}

MeshPartnershipAcceptPacketDescriptor::~MeshPartnershipAcceptPacketDescriptor()
{
}

bool MeshPartnershipAcceptPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MeshPartnershipAcceptPacket *>(obj)!=NULL;
}

const char *MeshPartnershipAcceptPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MeshPartnershipAcceptPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int MeshPartnershipAcceptPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *MeshPartnershipAcceptPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "upBw",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int MeshPartnershipAcceptPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='u' && strcmp(fieldName, "upBw")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MeshPartnershipAcceptPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *MeshPartnershipAcceptPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"default")) return "-1.0";
            return NULL;
        default: return NULL;
    }
}

int MeshPartnershipAcceptPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipAcceptPacket *pp = (MeshPartnershipAcceptPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MeshPartnershipAcceptPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipAcceptPacket *pp = (MeshPartnershipAcceptPacket *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getUpBw());
        default: return "";
    }
}

bool MeshPartnershipAcceptPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipAcceptPacket *pp = (MeshPartnershipAcceptPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setUpBw(string2double(value)); return true;
        default: return false;
    }
}

const char *MeshPartnershipAcceptPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *MeshPartnershipAcceptPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipAcceptPacket *pp = (MeshPartnershipAcceptPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MeshPartnershipRejectPacket);

MeshPartnershipRejectPacket::MeshPartnershipRejectPacket(const char *name, int kind) : MeshPeerStreamingPacket(name,kind)
{
    this->setPacketType(MESH_PARTNERSHIP_REJECT);
}

MeshPartnershipRejectPacket::MeshPartnershipRejectPacket(const MeshPartnershipRejectPacket& other) : MeshPeerStreamingPacket(other)
{
    copy(other);
}

MeshPartnershipRejectPacket::~MeshPartnershipRejectPacket()
{
}

MeshPartnershipRejectPacket& MeshPartnershipRejectPacket::operator=(const MeshPartnershipRejectPacket& other)
{
    if (this==&other) return *this;
    MeshPeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void MeshPartnershipRejectPacket::copy(const MeshPartnershipRejectPacket& other)
{
}

void MeshPartnershipRejectPacket::parsimPack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimPack(b);
}

void MeshPartnershipRejectPacket::parsimUnpack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimUnpack(b);
}

class MeshPartnershipRejectPacketDescriptor : public cClassDescriptor
{
  public:
    MeshPartnershipRejectPacketDescriptor();
    virtual ~MeshPartnershipRejectPacketDescriptor();

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

Register_ClassDescriptor(MeshPartnershipRejectPacketDescriptor);

MeshPartnershipRejectPacketDescriptor::MeshPartnershipRejectPacketDescriptor() : cClassDescriptor("MeshPartnershipRejectPacket", "MeshPeerStreamingPacket")
{
}

MeshPartnershipRejectPacketDescriptor::~MeshPartnershipRejectPacketDescriptor()
{
}

bool MeshPartnershipRejectPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MeshPartnershipRejectPacket *>(obj)!=NULL;
}

const char *MeshPartnershipRejectPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MeshPartnershipRejectPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int MeshPartnershipRejectPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *MeshPartnershipRejectPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int MeshPartnershipRejectPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MeshPartnershipRejectPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *MeshPartnershipRejectPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int MeshPartnershipRejectPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipRejectPacket *pp = (MeshPartnershipRejectPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MeshPartnershipRejectPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipRejectPacket *pp = (MeshPartnershipRejectPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool MeshPartnershipRejectPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipRejectPacket *pp = (MeshPartnershipRejectPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *MeshPartnershipRejectPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *MeshPartnershipRejectPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MeshPartnershipRejectPacket *pp = (MeshPartnershipRejectPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MeshBufferMapPacket);

MeshBufferMapPacket::MeshBufferMapPacket(const char *name, int kind) : MeshPeerStreamingPacket(name,kind)
{
    this->setPacketType(MESH_BUFFER_MAP);

    this->bmStart_seqNum_var = 0;
    this->bmEnd_seqNum_var = 0;
    this->head_seqNum_var = 0;
    bufferMap_arraysize = 0;
    this->bufferMap_var = 0;
}

MeshBufferMapPacket::MeshBufferMapPacket(const MeshBufferMapPacket& other) : MeshPeerStreamingPacket(other)
{
    bufferMap_arraysize = 0;
    this->bufferMap_var = 0;
    copy(other);
}

MeshBufferMapPacket::~MeshBufferMapPacket()
{
    delete [] bufferMap_var;
}

MeshBufferMapPacket& MeshBufferMapPacket::operator=(const MeshBufferMapPacket& other)
{
    if (this==&other) return *this;
    MeshPeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void MeshBufferMapPacket::copy(const MeshBufferMapPacket& other)
{
    this->bmStart_seqNum_var = other.bmStart_seqNum_var;
    this->bmEnd_seqNum_var = other.bmEnd_seqNum_var;
    this->head_seqNum_var = other.head_seqNum_var;
    delete [] this->bufferMap_var;
    this->bufferMap_var = (other.bufferMap_arraysize==0) ? NULL : new bool[other.bufferMap_arraysize];
    bufferMap_arraysize = other.bufferMap_arraysize;
    for (unsigned int i=0; i<bufferMap_arraysize; i++)
        this->bufferMap_var[i] = other.bufferMap_var[i];
}

void MeshBufferMapPacket::parsimPack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimPack(b);
    doPacking(b,this->bmStart_seqNum_var);
    doPacking(b,this->bmEnd_seqNum_var);
    doPacking(b,this->head_seqNum_var);
    b->pack(bufferMap_arraysize);
    doPacking(b,this->bufferMap_var,bufferMap_arraysize);
}

void MeshBufferMapPacket::parsimUnpack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->bmStart_seqNum_var);
    doUnpacking(b,this->bmEnd_seqNum_var);
    doUnpacking(b,this->head_seqNum_var);
    delete [] this->bufferMap_var;
    b->unpack(bufferMap_arraysize);
    if (bufferMap_arraysize==0) {
        this->bufferMap_var = 0;
    } else {
        this->bufferMap_var = new bool[bufferMap_arraysize];
        doUnpacking(b,this->bufferMap_var,bufferMap_arraysize);
    }
}

long MeshBufferMapPacket::getBmStartSeqNum() const
{
    return bmStart_seqNum_var;
}

void MeshBufferMapPacket::setBmStartSeqNum(long bmStart_seqNum)
{
    this->bmStart_seqNum_var = bmStart_seqNum;
}

long MeshBufferMapPacket::getBmEndSeqNum() const
{
    return bmEnd_seqNum_var;
}

void MeshBufferMapPacket::setBmEndSeqNum(long bmEnd_seqNum)
{
    this->bmEnd_seqNum_var = bmEnd_seqNum;
}

long MeshBufferMapPacket::getHeadSeqNum() const
{
    return head_seqNum_var;
}

void MeshBufferMapPacket::setHeadSeqNum(long head_seqNum)
{
    this->head_seqNum_var = head_seqNum;
}

void MeshBufferMapPacket::setBufferMapArraySize(unsigned int size)
{
    bool *bufferMap_var2 = (size==0) ? NULL : new bool[size];
    unsigned int sz = bufferMap_arraysize < size ? bufferMap_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        bufferMap_var2[i] = this->bufferMap_var[i];
    for (unsigned int i=sz; i<size; i++)
        bufferMap_var2[i] = 0;
    bufferMap_arraysize = size;
    delete [] this->bufferMap_var;
    this->bufferMap_var = bufferMap_var2;
}

unsigned int MeshBufferMapPacket::getBufferMapArraySize() const
{
    return bufferMap_arraysize;
}

bool MeshBufferMapPacket::getBufferMap(unsigned int k) const
{
    if (k>=bufferMap_arraysize) throw cRuntimeError("Array of size %d indexed by %d", bufferMap_arraysize, k);
    return bufferMap_var[k];
}

void MeshBufferMapPacket::setBufferMap(unsigned int k, bool bufferMap)
{
    if (k>=bufferMap_arraysize) throw cRuntimeError("Array of size %d indexed by %d", bufferMap_arraysize, k);
    this->bufferMap_var[k] = bufferMap;
}

class MeshBufferMapPacketDescriptor : public cClassDescriptor
{
  public:
    MeshBufferMapPacketDescriptor();
    virtual ~MeshBufferMapPacketDescriptor();

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

Register_ClassDescriptor(MeshBufferMapPacketDescriptor);

MeshBufferMapPacketDescriptor::MeshBufferMapPacketDescriptor() : cClassDescriptor("MeshBufferMapPacket", "MeshPeerStreamingPacket")
{
}

MeshBufferMapPacketDescriptor::~MeshBufferMapPacketDescriptor()
{
}

bool MeshBufferMapPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MeshBufferMapPacket *>(obj)!=NULL;
}

const char *MeshBufferMapPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MeshBufferMapPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int MeshBufferMapPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *MeshBufferMapPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "bmStart_seqNum",
        "bmEnd_seqNum",
        "head_seqNum",
        "bufferMap",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int MeshBufferMapPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bmStart_seqNum")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bmEnd_seqNum")==0) return base+1;
    if (fieldName[0]=='h' && strcmp(fieldName, "head_seqNum")==0) return base+2;
    if (fieldName[0]=='b' && strcmp(fieldName, "bufferMap")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MeshBufferMapPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "long",
        "long",
        "long",
        "bool",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *MeshBufferMapPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"getter")) return "getBmStartSeqNum";
            if (!strcmp(propertyname,"setter")) return "setBmStartSeqNum";
            return NULL;
        case 1:
            if (!strcmp(propertyname,"getter")) return "getBmEndSeqNum";
            if (!strcmp(propertyname,"setter")) return "setBmEndSeqNum";
            return NULL;
        case 2:
            if (!strcmp(propertyname,"getter")) return "getHeadSeqNum";
            if (!strcmp(propertyname,"setter")) return "setHeadSeqNum";
            return NULL;
        default: return NULL;
    }
}

int MeshBufferMapPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MeshBufferMapPacket *pp = (MeshBufferMapPacket *)object; (void)pp;
    switch (field) {
        case 3: return pp->getBufferMapArraySize();
        default: return 0;
    }
}

std::string MeshBufferMapPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MeshBufferMapPacket *pp = (MeshBufferMapPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getBmStartSeqNum());
        case 1: return long2string(pp->getBmEndSeqNum());
        case 2: return long2string(pp->getHeadSeqNum());
        case 3: return bool2string(pp->getBufferMap(i));
        default: return "";
    }
}

bool MeshBufferMapPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MeshBufferMapPacket *pp = (MeshBufferMapPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setBmStartSeqNum(string2long(value)); return true;
        case 1: pp->setBmEndSeqNum(string2long(value)); return true;
        case 2: pp->setHeadSeqNum(string2long(value)); return true;
        case 3: pp->setBufferMap(i,string2bool(value)); return true;
        default: return false;
    }
}

const char *MeshBufferMapPacketDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *MeshBufferMapPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MeshBufferMapPacket *pp = (MeshBufferMapPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MeshChunkRequestPacket);

MeshChunkRequestPacket::MeshChunkRequestPacket(const char *name, int kind) : MeshPeerStreamingPacket(name,kind)
{
    this->setPacketType(MESH_CHUNK_REQUEST);

    this->seqNum_mapStart_var = 0;
    this->seqNum_mapEnd_var = 0;
    this->seqNum_mapHead_var = 0;
    requestMap_arraysize = 0;
    this->requestMap_var = 0;
}

MeshChunkRequestPacket::MeshChunkRequestPacket(const MeshChunkRequestPacket& other) : MeshPeerStreamingPacket(other)
{
    requestMap_arraysize = 0;
    this->requestMap_var = 0;
    copy(other);
}

MeshChunkRequestPacket::~MeshChunkRequestPacket()
{
    delete [] requestMap_var;
}

MeshChunkRequestPacket& MeshChunkRequestPacket::operator=(const MeshChunkRequestPacket& other)
{
    if (this==&other) return *this;
    MeshPeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void MeshChunkRequestPacket::copy(const MeshChunkRequestPacket& other)
{
    this->seqNum_mapStart_var = other.seqNum_mapStart_var;
    this->seqNum_mapEnd_var = other.seqNum_mapEnd_var;
    this->seqNum_mapHead_var = other.seqNum_mapHead_var;
    delete [] this->requestMap_var;
    this->requestMap_var = (other.requestMap_arraysize==0) ? NULL : new bool[other.requestMap_arraysize];
    requestMap_arraysize = other.requestMap_arraysize;
    for (unsigned int i=0; i<requestMap_arraysize; i++)
        this->requestMap_var[i] = other.requestMap_var[i];
}

void MeshChunkRequestPacket::parsimPack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimPack(b);
    doPacking(b,this->seqNum_mapStart_var);
    doPacking(b,this->seqNum_mapEnd_var);
    doPacking(b,this->seqNum_mapHead_var);
    b->pack(requestMap_arraysize);
    doPacking(b,this->requestMap_var,requestMap_arraysize);
}

void MeshChunkRequestPacket::parsimUnpack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->seqNum_mapStart_var);
    doUnpacking(b,this->seqNum_mapEnd_var);
    doUnpacking(b,this->seqNum_mapHead_var);
    delete [] this->requestMap_var;
    b->unpack(requestMap_arraysize);
    if (requestMap_arraysize==0) {
        this->requestMap_var = 0;
    } else {
        this->requestMap_var = new bool[requestMap_arraysize];
        doUnpacking(b,this->requestMap_var,requestMap_arraysize);
    }
}

long MeshChunkRequestPacket::getSeqNumMapStart() const
{
    return seqNum_mapStart_var;
}

void MeshChunkRequestPacket::setSeqNumMapStart(long seqNum_mapStart)
{
    this->seqNum_mapStart_var = seqNum_mapStart;
}

long MeshChunkRequestPacket::getSeqNumMapEnd() const
{
    return seqNum_mapEnd_var;
}

void MeshChunkRequestPacket::setSeqNumMapEnd(long seqNum_mapEnd)
{
    this->seqNum_mapEnd_var = seqNum_mapEnd;
}

long MeshChunkRequestPacket::getSeqNumMapHead() const
{
    return seqNum_mapHead_var;
}

void MeshChunkRequestPacket::setSeqNumMapHead(long seqNum_mapHead)
{
    this->seqNum_mapHead_var = seqNum_mapHead;
}

void MeshChunkRequestPacket::setRequestMapArraySize(unsigned int size)
{
    bool *requestMap_var2 = (size==0) ? NULL : new bool[size];
    unsigned int sz = requestMap_arraysize < size ? requestMap_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        requestMap_var2[i] = this->requestMap_var[i];
    for (unsigned int i=sz; i<size; i++)
        requestMap_var2[i] = 0;
    requestMap_arraysize = size;
    delete [] this->requestMap_var;
    this->requestMap_var = requestMap_var2;
}

unsigned int MeshChunkRequestPacket::getRequestMapArraySize() const
{
    return requestMap_arraysize;
}

bool MeshChunkRequestPacket::getRequestMap(unsigned int k) const
{
    if (k>=requestMap_arraysize) throw cRuntimeError("Array of size %d indexed by %d", requestMap_arraysize, k);
    return requestMap_var[k];
}

void MeshChunkRequestPacket::setRequestMap(unsigned int k, bool requestMap)
{
    if (k>=requestMap_arraysize) throw cRuntimeError("Array of size %d indexed by %d", requestMap_arraysize, k);
    this->requestMap_var[k] = requestMap;
}

class MeshChunkRequestPacketDescriptor : public cClassDescriptor
{
  public:
    MeshChunkRequestPacketDescriptor();
    virtual ~MeshChunkRequestPacketDescriptor();

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

Register_ClassDescriptor(MeshChunkRequestPacketDescriptor);

MeshChunkRequestPacketDescriptor::MeshChunkRequestPacketDescriptor() : cClassDescriptor("MeshChunkRequestPacket", "MeshPeerStreamingPacket")
{
}

MeshChunkRequestPacketDescriptor::~MeshChunkRequestPacketDescriptor()
{
}

bool MeshChunkRequestPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MeshChunkRequestPacket *>(obj)!=NULL;
}

const char *MeshChunkRequestPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MeshChunkRequestPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int MeshChunkRequestPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *MeshChunkRequestPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "seqNum_mapStart",
        "seqNum_mapEnd",
        "seqNum_mapHead",
        "requestMap",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int MeshChunkRequestPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqNum_mapStart")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqNum_mapEnd")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqNum_mapHead")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "requestMap")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MeshChunkRequestPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "long",
        "long",
        "long",
        "bool",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *MeshChunkRequestPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"getter")) return "getSeqNumMapStart";
            if (!strcmp(propertyname,"setter")) return "setSeqNumMapStart";
            return NULL;
        case 1:
            if (!strcmp(propertyname,"getter")) return "getSeqNumMapEnd";
            if (!strcmp(propertyname,"setter")) return "setSeqNumMapEnd";
            return NULL;
        case 2:
            if (!strcmp(propertyname,"getter")) return "getSeqNumMapHead";
            if (!strcmp(propertyname,"setter")) return "setSeqNumMapHead";
            return NULL;
        default: return NULL;
    }
}

int MeshChunkRequestPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MeshChunkRequestPacket *pp = (MeshChunkRequestPacket *)object; (void)pp;
    switch (field) {
        case 3: return pp->getRequestMapArraySize();
        default: return 0;
    }
}

std::string MeshChunkRequestPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MeshChunkRequestPacket *pp = (MeshChunkRequestPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSeqNumMapStart());
        case 1: return long2string(pp->getSeqNumMapEnd());
        case 2: return long2string(pp->getSeqNumMapHead());
        case 3: return bool2string(pp->getRequestMap(i));
        default: return "";
    }
}

bool MeshChunkRequestPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MeshChunkRequestPacket *pp = (MeshChunkRequestPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setSeqNumMapStart(string2long(value)); return true;
        case 1: pp->setSeqNumMapEnd(string2long(value)); return true;
        case 2: pp->setSeqNumMapHead(string2long(value)); return true;
        case 3: pp->setRequestMap(i,string2bool(value)); return true;
        default: return false;
    }
}

const char *MeshChunkRequestPacketDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *MeshChunkRequestPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MeshChunkRequestPacket *pp = (MeshChunkRequestPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



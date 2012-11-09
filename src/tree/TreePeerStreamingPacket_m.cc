//
// Generated file, do not edit! Created by opp_msgc 4.2 from applications/common/messages/TreePeerStreamingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "TreePeerStreamingPacket_m.h"

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
    cEnum *e = cEnum::find("TreePeerStreamingPacketType");
    if (!e) enums.getInstance()->add(e = new cEnum("TreePeerStreamingPacketType"));
    e->insert(TREE_KEEP_ALIVE, "TREE_KEEP_ALIVE");
);

Register_Class(TreePeerStreamingPacket);

TreePeerStreamingPacket::TreePeerStreamingPacket(const char *name, int kind) : PeerStreamingPacket(name,kind)
{
    this->setPacketGroup(PACKET_GROUP_TREE_OVERLAY);

    this->packetType_var = 0;
}

TreePeerStreamingPacket::TreePeerStreamingPacket(const TreePeerStreamingPacket& other) : PeerStreamingPacket(other)
{
    copy(other);
}

TreePeerStreamingPacket::~TreePeerStreamingPacket()
{
}

TreePeerStreamingPacket& TreePeerStreamingPacket::operator=(const TreePeerStreamingPacket& other)
{
    if (this==&other) return *this;
    PeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void TreePeerStreamingPacket::copy(const TreePeerStreamingPacket& other)
{
    this->packetType_var = other.packetType_var;
}

void TreePeerStreamingPacket::parsimPack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
}

void TreePeerStreamingPacket::parsimUnpack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
}

short TreePeerStreamingPacket::getPacketType() const
{
    return packetType_var;
}

void TreePeerStreamingPacket::setPacketType(short packetType)
{
    this->packetType_var = packetType;
}

class TreePeerStreamingPacketDescriptor : public cClassDescriptor
{
  public:
    TreePeerStreamingPacketDescriptor();
    virtual ~TreePeerStreamingPacketDescriptor();

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

Register_ClassDescriptor(TreePeerStreamingPacketDescriptor);

TreePeerStreamingPacketDescriptor::TreePeerStreamingPacketDescriptor() : cClassDescriptor("TreePeerStreamingPacket", "PeerStreamingPacket")
{
}

TreePeerStreamingPacketDescriptor::~TreePeerStreamingPacketDescriptor()
{
}

bool TreePeerStreamingPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<TreePeerStreamingPacket *>(obj)!=NULL;
}

const char *TreePeerStreamingPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int TreePeerStreamingPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int TreePeerStreamingPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *TreePeerStreamingPacketDescriptor::getFieldName(void *object, int field) const
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

int TreePeerStreamingPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *TreePeerStreamingPacketDescriptor::getFieldTypeString(void *object, int field) const
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

const char *TreePeerStreamingPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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
            if (!strcmp(propertyname,"enum")) return "TreePeerStreamingPacketType";
            if (!strcmp(propertyname,"setter")) return "setPacketType";
            return NULL;
        default: return NULL;
    }
}

int TreePeerStreamingPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    TreePeerStreamingPacket *pp = (TreePeerStreamingPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string TreePeerStreamingPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    TreePeerStreamingPacket *pp = (TreePeerStreamingPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPacketType());
        default: return "";
    }
}

bool TreePeerStreamingPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    TreePeerStreamingPacket *pp = (TreePeerStreamingPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2long(value)); return true;
        default: return false;
    }
}

const char *TreePeerStreamingPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *TreePeerStreamingPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    TreePeerStreamingPacket *pp = (TreePeerStreamingPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



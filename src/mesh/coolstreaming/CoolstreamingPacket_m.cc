//  
// =============================================================================
// OSSIM : A Generic Simulation Framework for Overlay Streaming
// =============================================================================
//
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Project Info: http://www.p2p.tu-darmstadt.de/research/ossim
//
// OSSIM is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// OSSIM is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with 
// this program. If not, see <http://www.gnu.org/licenses/>.

// -----------------------------------------------------------------------------
// CoolstreamingPacket_m.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// ------------------------------------------------------------------------------
//

//
// Generated file, do not edit! Created by opp_msgc 4.2 from mesh/coolstreaming/CoolstreamingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "CoolstreamingPacket_m.h"

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
    cEnum *e = cEnum::find("CoolstreamingPacketType");
    if (!e) enums.getInstance()->add(e = new cEnum("CoolstreamingPacketType"));
    e->insert(COOLSTREAMING_BUFFER_MAP, "COOLSTREAMING_BUFFER_MAP");
    e->insert(COOLSTREAMING_PARTNERSHIP_REQUEST, "COOLSTREAMING_PARTNERSHIP_REQUEST");
    e->insert(COOLSTREAMING_PARTNERSHIP_ACCEPT, "COOLSTREAMING_PARTNERSHIP_ACCEPT");
    e->insert(COOLSTREAMING_PARTNERSHIP_REVOKE, "COOLSTREAMING_PARTNERSHIP_REVOKE");
    e->insert(COOLSTREAMING_CHUNK, "COOLSTREAMING_CHUNK");
);

Register_Class(CoolstreamingPacket);

CoolstreamingPacket::CoolstreamingPacket(const char *name, int kind) : MeshPeerStreamingPacket(name,kind)
{
    this->setPacketGroup(PACKET_GROUP_MESH_OVERLAY);

    this->packetType_var = 0;
}

CoolstreamingPacket::CoolstreamingPacket(const CoolstreamingPacket& other) : MeshPeerStreamingPacket(other)
{
    copy(other);
}

CoolstreamingPacket::~CoolstreamingPacket()
{
}

CoolstreamingPacket& CoolstreamingPacket::operator=(const CoolstreamingPacket& other)
{
    if (this==&other) return *this;
    MeshPeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void CoolstreamingPacket::copy(const CoolstreamingPacket& other)
{
    this->packetType_var = other.packetType_var;
}

void CoolstreamingPacket::parsimPack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
}

void CoolstreamingPacket::parsimUnpack(cCommBuffer *b)
{
    MeshPeerStreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
}

short CoolstreamingPacket::getPacketType() const
{
    return packetType_var;
}

void CoolstreamingPacket::setPacketType(short packetType)
{
    this->packetType_var = packetType;
}

class CoolstreamingPacketDescriptor : public cClassDescriptor
{
  public:
    CoolstreamingPacketDescriptor();
    virtual ~CoolstreamingPacketDescriptor();

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

Register_ClassDescriptor(CoolstreamingPacketDescriptor);

CoolstreamingPacketDescriptor::CoolstreamingPacketDescriptor() : cClassDescriptor("CoolstreamingPacket", "MeshPeerStreamingPacket")
{
}

CoolstreamingPacketDescriptor::~CoolstreamingPacketDescriptor()
{
}

bool CoolstreamingPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CoolstreamingPacket *>(obj)!=NULL;
}

const char *CoolstreamingPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CoolstreamingPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int CoolstreamingPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *CoolstreamingPacketDescriptor::getFieldName(void *object, int field) const
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

int CoolstreamingPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CoolstreamingPacketDescriptor::getFieldTypeString(void *object, int field) const
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

const char *CoolstreamingPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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
            if (!strcmp(propertyname,"enum")) return "CoolstreamingPacketType";
            if (!strcmp(propertyname,"setter")) return "setPacketType";
            return NULL;
        default: return NULL;
    }
}

int CoolstreamingPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPacket *pp = (CoolstreamingPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CoolstreamingPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPacket *pp = (CoolstreamingPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPacketType());
        default: return "";
    }
}

bool CoolstreamingPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPacket *pp = (CoolstreamingPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2long(value)); return true;
        default: return false;
    }
}

const char *CoolstreamingPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *CoolstreamingPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPacket *pp = (CoolstreamingPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CoolstreamingBufferMapPacket);

CoolstreamingBufferMapPacket::CoolstreamingBufferMapPacket(const char *name, int kind) : CoolstreamingPacket(name,kind)
{
    this->setPacketType(COOLSTREAMING_BUFFER_MAP);

    sequenceNumbers_arraysize = 0;
    this->sequenceNumbers_var = 0;
    subscribe_arraysize = 0;
    this->subscribe_var = 0;
}

CoolstreamingBufferMapPacket::CoolstreamingBufferMapPacket(const CoolstreamingBufferMapPacket& other) : CoolstreamingPacket(other)
{
    sequenceNumbers_arraysize = 0;
    this->sequenceNumbers_var = 0;
    subscribe_arraysize = 0;
    this->subscribe_var = 0;
    copy(other);
}

CoolstreamingBufferMapPacket::~CoolstreamingBufferMapPacket()
{
    delete [] sequenceNumbers_var;
    delete [] subscribe_var;
}

CoolstreamingBufferMapPacket& CoolstreamingBufferMapPacket::operator=(const CoolstreamingBufferMapPacket& other)
{
    if (this==&other) return *this;
    CoolstreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void CoolstreamingBufferMapPacket::copy(const CoolstreamingBufferMapPacket& other)
{
    delete [] this->sequenceNumbers_var;
    this->sequenceNumbers_var = (other.sequenceNumbers_arraysize==0) ? NULL : new int[other.sequenceNumbers_arraysize];
    sequenceNumbers_arraysize = other.sequenceNumbers_arraysize;
    for (unsigned int i=0; i<sequenceNumbers_arraysize; i++)
        this->sequenceNumbers_var[i] = other.sequenceNumbers_var[i];
    delete [] this->subscribe_var;
    this->subscribe_var = (other.subscribe_arraysize==0) ? NULL : new bool[other.subscribe_arraysize];
    subscribe_arraysize = other.subscribe_arraysize;
    for (unsigned int i=0; i<subscribe_arraysize; i++)
        this->subscribe_var[i] = other.subscribe_var[i];
}

void CoolstreamingBufferMapPacket::parsimPack(cCommBuffer *b)
{
    CoolstreamingPacket::parsimPack(b);
    b->pack(sequenceNumbers_arraysize);
    doPacking(b,this->sequenceNumbers_var,sequenceNumbers_arraysize);
    b->pack(subscribe_arraysize);
    doPacking(b,this->subscribe_var,subscribe_arraysize);
}

void CoolstreamingBufferMapPacket::parsimUnpack(cCommBuffer *b)
{
    CoolstreamingPacket::parsimUnpack(b);
    delete [] this->sequenceNumbers_var;
    b->unpack(sequenceNumbers_arraysize);
    if (sequenceNumbers_arraysize==0) {
        this->sequenceNumbers_var = 0;
    } else {
        this->sequenceNumbers_var = new int[sequenceNumbers_arraysize];
        doUnpacking(b,this->sequenceNumbers_var,sequenceNumbers_arraysize);
    }
    delete [] this->subscribe_var;
    b->unpack(subscribe_arraysize);
    if (subscribe_arraysize==0) {
        this->subscribe_var = 0;
    } else {
        this->subscribe_var = new bool[subscribe_arraysize];
        doUnpacking(b,this->subscribe_var,subscribe_arraysize);
    }
}

void CoolstreamingBufferMapPacket::setSequenceNumbersArraySize(unsigned int size)
{
    int *sequenceNumbers_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = sequenceNumbers_arraysize < size ? sequenceNumbers_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        sequenceNumbers_var2[i] = this->sequenceNumbers_var[i];
    for (unsigned int i=sz; i<size; i++)
        sequenceNumbers_var2[i] = 0;
    sequenceNumbers_arraysize = size;
    delete [] this->sequenceNumbers_var;
    this->sequenceNumbers_var = sequenceNumbers_var2;
}

unsigned int CoolstreamingBufferMapPacket::getSequenceNumbersArraySize() const
{
    return sequenceNumbers_arraysize;
}

int CoolstreamingBufferMapPacket::getSequenceNumbers(unsigned int k) const
{
    if (k>=sequenceNumbers_arraysize) throw cRuntimeError("Array of size %d indexed by %d", sequenceNumbers_arraysize, k);
    return sequenceNumbers_var[k];
}

void CoolstreamingBufferMapPacket::setSequenceNumbers(unsigned int k, int sequenceNumbers)
{
    if (k>=sequenceNumbers_arraysize) throw cRuntimeError("Array of size %d indexed by %d", sequenceNumbers_arraysize, k);
    this->sequenceNumbers_var[k] = sequenceNumbers;
}

void CoolstreamingBufferMapPacket::setSubscribeArraySize(unsigned int size)
{
    bool *subscribe_var2 = (size==0) ? NULL : new bool[size];
    unsigned int sz = subscribe_arraysize < size ? subscribe_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        subscribe_var2[i] = this->subscribe_var[i];
    for (unsigned int i=sz; i<size; i++)
        subscribe_var2[i] = 0;
    subscribe_arraysize = size;
    delete [] this->subscribe_var;
    this->subscribe_var = subscribe_var2;
}

unsigned int CoolstreamingBufferMapPacket::getSubscribeArraySize() const
{
    return subscribe_arraysize;
}

bool CoolstreamingBufferMapPacket::getSubscribe(unsigned int k) const
{
    if (k>=subscribe_arraysize) throw cRuntimeError("Array of size %d indexed by %d", subscribe_arraysize, k);
    return subscribe_var[k];
}

void CoolstreamingBufferMapPacket::setSubscribe(unsigned int k, bool subscribe)
{
    if (k>=subscribe_arraysize) throw cRuntimeError("Array of size %d indexed by %d", subscribe_arraysize, k);
    this->subscribe_var[k] = subscribe;
}

class CoolstreamingBufferMapPacketDescriptor : public cClassDescriptor
{
  public:
    CoolstreamingBufferMapPacketDescriptor();
    virtual ~CoolstreamingBufferMapPacketDescriptor();

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

Register_ClassDescriptor(CoolstreamingBufferMapPacketDescriptor);

CoolstreamingBufferMapPacketDescriptor::CoolstreamingBufferMapPacketDescriptor() : cClassDescriptor("CoolstreamingBufferMapPacket", "CoolstreamingPacket")
{
}

CoolstreamingBufferMapPacketDescriptor::~CoolstreamingBufferMapPacketDescriptor()
{
}

bool CoolstreamingBufferMapPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CoolstreamingBufferMapPacket *>(obj)!=NULL;
}

const char *CoolstreamingBufferMapPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CoolstreamingBufferMapPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int CoolstreamingBufferMapPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *CoolstreamingBufferMapPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "sequenceNumbers",
        "subscribe",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int CoolstreamingBufferMapPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumbers")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "subscribe")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CoolstreamingBufferMapPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "bool",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *CoolstreamingBufferMapPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CoolstreamingBufferMapPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingBufferMapPacket *pp = (CoolstreamingBufferMapPacket *)object; (void)pp;
    switch (field) {
        case 0: return pp->getSequenceNumbersArraySize();
        case 1: return pp->getSubscribeArraySize();
        default: return 0;
    }
}

std::string CoolstreamingBufferMapPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingBufferMapPacket *pp = (CoolstreamingBufferMapPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSequenceNumbers(i));
        case 1: return bool2string(pp->getSubscribe(i));
        default: return "";
    }
}

bool CoolstreamingBufferMapPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingBufferMapPacket *pp = (CoolstreamingBufferMapPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setSequenceNumbers(i,string2long(value)); return true;
        case 1: pp->setSubscribe(i,string2bool(value)); return true;
        default: return false;
    }
}

const char *CoolstreamingBufferMapPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *CoolstreamingBufferMapPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingBufferMapPacket *pp = (CoolstreamingBufferMapPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CoolstreamingPartnershipRequestPacket);

CoolstreamingPartnershipRequestPacket::CoolstreamingPartnershipRequestPacket(const char *name, int kind) : CoolstreamingPacket(name,kind)
{
    this->setPacketType(COOLSTREAMING_PARTNERSHIP_REQUEST);
}

CoolstreamingPartnershipRequestPacket::CoolstreamingPartnershipRequestPacket(const CoolstreamingPartnershipRequestPacket& other) : CoolstreamingPacket(other)
{
    copy(other);
}

CoolstreamingPartnershipRequestPacket::~CoolstreamingPartnershipRequestPacket()
{
}

CoolstreamingPartnershipRequestPacket& CoolstreamingPartnershipRequestPacket::operator=(const CoolstreamingPartnershipRequestPacket& other)
{
    if (this==&other) return *this;
    CoolstreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void CoolstreamingPartnershipRequestPacket::copy(const CoolstreamingPartnershipRequestPacket& other)
{
}

void CoolstreamingPartnershipRequestPacket::parsimPack(cCommBuffer *b)
{
    CoolstreamingPacket::parsimPack(b);
}

void CoolstreamingPartnershipRequestPacket::parsimUnpack(cCommBuffer *b)
{
    CoolstreamingPacket::parsimUnpack(b);
}

class CoolstreamingPartnershipRequestPacketDescriptor : public cClassDescriptor
{
  public:
    CoolstreamingPartnershipRequestPacketDescriptor();
    virtual ~CoolstreamingPartnershipRequestPacketDescriptor();

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

Register_ClassDescriptor(CoolstreamingPartnershipRequestPacketDescriptor);

CoolstreamingPartnershipRequestPacketDescriptor::CoolstreamingPartnershipRequestPacketDescriptor() : cClassDescriptor("CoolstreamingPartnershipRequestPacket", "CoolstreamingPacket")
{
}

CoolstreamingPartnershipRequestPacketDescriptor::~CoolstreamingPartnershipRequestPacketDescriptor()
{
}

bool CoolstreamingPartnershipRequestPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CoolstreamingPartnershipRequestPacket *>(obj)!=NULL;
}

const char *CoolstreamingPartnershipRequestPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CoolstreamingPartnershipRequestPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int CoolstreamingPartnershipRequestPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *CoolstreamingPartnershipRequestPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int CoolstreamingPartnershipRequestPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CoolstreamingPartnershipRequestPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *CoolstreamingPartnershipRequestPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CoolstreamingPartnershipRequestPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipRequestPacket *pp = (CoolstreamingPartnershipRequestPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CoolstreamingPartnershipRequestPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipRequestPacket *pp = (CoolstreamingPartnershipRequestPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool CoolstreamingPartnershipRequestPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipRequestPacket *pp = (CoolstreamingPartnershipRequestPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *CoolstreamingPartnershipRequestPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *CoolstreamingPartnershipRequestPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipRequestPacket *pp = (CoolstreamingPartnershipRequestPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CoolstreamingPartnershipAcceptPacket);

CoolstreamingPartnershipAcceptPacket::CoolstreamingPartnershipAcceptPacket(const char *name, int kind) : CoolstreamingPacket(name,kind)
{
    this->setPacketType(COOLSTREAMING_PARTNERSHIP_ACCEPT);
}

CoolstreamingPartnershipAcceptPacket::CoolstreamingPartnershipAcceptPacket(const CoolstreamingPartnershipAcceptPacket& other) : CoolstreamingPacket(other)
{
    copy(other);
}

CoolstreamingPartnershipAcceptPacket::~CoolstreamingPartnershipAcceptPacket()
{
}

CoolstreamingPartnershipAcceptPacket& CoolstreamingPartnershipAcceptPacket::operator=(const CoolstreamingPartnershipAcceptPacket& other)
{
    if (this==&other) return *this;
    CoolstreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void CoolstreamingPartnershipAcceptPacket::copy(const CoolstreamingPartnershipAcceptPacket& other)
{
}

void CoolstreamingPartnershipAcceptPacket::parsimPack(cCommBuffer *b)
{
    CoolstreamingPacket::parsimPack(b);
}

void CoolstreamingPartnershipAcceptPacket::parsimUnpack(cCommBuffer *b)
{
    CoolstreamingPacket::parsimUnpack(b);
}

class CoolstreamingPartnershipAcceptPacketDescriptor : public cClassDescriptor
{
  public:
    CoolstreamingPartnershipAcceptPacketDescriptor();
    virtual ~CoolstreamingPartnershipAcceptPacketDescriptor();

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

Register_ClassDescriptor(CoolstreamingPartnershipAcceptPacketDescriptor);

CoolstreamingPartnershipAcceptPacketDescriptor::CoolstreamingPartnershipAcceptPacketDescriptor() : cClassDescriptor("CoolstreamingPartnershipAcceptPacket", "CoolstreamingPacket")
{
}

CoolstreamingPartnershipAcceptPacketDescriptor::~CoolstreamingPartnershipAcceptPacketDescriptor()
{
}

bool CoolstreamingPartnershipAcceptPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CoolstreamingPartnershipAcceptPacket *>(obj)!=NULL;
}

const char *CoolstreamingPartnershipAcceptPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CoolstreamingPartnershipAcceptPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int CoolstreamingPartnershipAcceptPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *CoolstreamingPartnershipAcceptPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int CoolstreamingPartnershipAcceptPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CoolstreamingPartnershipAcceptPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *CoolstreamingPartnershipAcceptPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CoolstreamingPartnershipAcceptPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipAcceptPacket *pp = (CoolstreamingPartnershipAcceptPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CoolstreamingPartnershipAcceptPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipAcceptPacket *pp = (CoolstreamingPartnershipAcceptPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool CoolstreamingPartnershipAcceptPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipAcceptPacket *pp = (CoolstreamingPartnershipAcceptPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *CoolstreamingPartnershipAcceptPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *CoolstreamingPartnershipAcceptPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipAcceptPacket *pp = (CoolstreamingPartnershipAcceptPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CoolstreamingPartnershipRevokePacket);

CoolstreamingPartnershipRevokePacket::CoolstreamingPartnershipRevokePacket(const char *name, int kind) : CoolstreamingPacket(name,kind)
{
    this->setPacketType(COOLSTREAMING_PARTNERSHIP_REVOKE);
}

CoolstreamingPartnershipRevokePacket::CoolstreamingPartnershipRevokePacket(const CoolstreamingPartnershipRevokePacket& other) : CoolstreamingPacket(other)
{
    copy(other);
}

CoolstreamingPartnershipRevokePacket::~CoolstreamingPartnershipRevokePacket()
{
}

CoolstreamingPartnershipRevokePacket& CoolstreamingPartnershipRevokePacket::operator=(const CoolstreamingPartnershipRevokePacket& other)
{
    if (this==&other) return *this;
    CoolstreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void CoolstreamingPartnershipRevokePacket::copy(const CoolstreamingPartnershipRevokePacket& other)
{
}

void CoolstreamingPartnershipRevokePacket::parsimPack(cCommBuffer *b)
{
    CoolstreamingPacket::parsimPack(b);
}

void CoolstreamingPartnershipRevokePacket::parsimUnpack(cCommBuffer *b)
{
    CoolstreamingPacket::parsimUnpack(b);
}

class CoolstreamingPartnershipRevokePacketDescriptor : public cClassDescriptor
{
  public:
    CoolstreamingPartnershipRevokePacketDescriptor();
    virtual ~CoolstreamingPartnershipRevokePacketDescriptor();

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

Register_ClassDescriptor(CoolstreamingPartnershipRevokePacketDescriptor);

CoolstreamingPartnershipRevokePacketDescriptor::CoolstreamingPartnershipRevokePacketDescriptor() : cClassDescriptor("CoolstreamingPartnershipRevokePacket", "CoolstreamingPacket")
{
}

CoolstreamingPartnershipRevokePacketDescriptor::~CoolstreamingPartnershipRevokePacketDescriptor()
{
}

bool CoolstreamingPartnershipRevokePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CoolstreamingPartnershipRevokePacket *>(obj)!=NULL;
}

const char *CoolstreamingPartnershipRevokePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CoolstreamingPartnershipRevokePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int CoolstreamingPartnershipRevokePacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *CoolstreamingPartnershipRevokePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int CoolstreamingPartnershipRevokePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CoolstreamingPartnershipRevokePacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *CoolstreamingPartnershipRevokePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CoolstreamingPartnershipRevokePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipRevokePacket *pp = (CoolstreamingPartnershipRevokePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CoolstreamingPartnershipRevokePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipRevokePacket *pp = (CoolstreamingPartnershipRevokePacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool CoolstreamingPartnershipRevokePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipRevokePacket *pp = (CoolstreamingPartnershipRevokePacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *CoolstreamingPartnershipRevokePacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *CoolstreamingPartnershipRevokePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingPartnershipRevokePacket *pp = (CoolstreamingPartnershipRevokePacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CoolstreamingChunkPacket);

CoolstreamingChunkPacket::CoolstreamingChunkPacket(const char *name, int kind) : CoolstreamingPacket(name,kind)
{
    this->setPacketType(COOLSTREAMING_CHUNK);

    this->sequenceNumber_var = 0;
}

CoolstreamingChunkPacket::CoolstreamingChunkPacket(const CoolstreamingChunkPacket& other) : CoolstreamingPacket(other)
{
    copy(other);
}

CoolstreamingChunkPacket::~CoolstreamingChunkPacket()
{
}

CoolstreamingChunkPacket& CoolstreamingChunkPacket::operator=(const CoolstreamingChunkPacket& other)
{
    if (this==&other) return *this;
    CoolstreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void CoolstreamingChunkPacket::copy(const CoolstreamingChunkPacket& other)
{
    this->sequenceNumber_var = other.sequenceNumber_var;
}

void CoolstreamingChunkPacket::parsimPack(cCommBuffer *b)
{
    CoolstreamingPacket::parsimPack(b);
    doPacking(b,this->sequenceNumber_var);
}

void CoolstreamingChunkPacket::parsimUnpack(cCommBuffer *b)
{
    CoolstreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->sequenceNumber_var);
}

int CoolstreamingChunkPacket::getSequenceNumber() const
{
    return sequenceNumber_var;
}

void CoolstreamingChunkPacket::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber_var = sequenceNumber;
}

class CoolstreamingChunkPacketDescriptor : public cClassDescriptor
{
  public:
    CoolstreamingChunkPacketDescriptor();
    virtual ~CoolstreamingChunkPacketDescriptor();

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

Register_ClassDescriptor(CoolstreamingChunkPacketDescriptor);

CoolstreamingChunkPacketDescriptor::CoolstreamingChunkPacketDescriptor() : cClassDescriptor("CoolstreamingChunkPacket", "CoolstreamingPacket")
{
}

CoolstreamingChunkPacketDescriptor::~CoolstreamingChunkPacketDescriptor()
{
}

bool CoolstreamingChunkPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CoolstreamingChunkPacket *>(obj)!=NULL;
}

const char *CoolstreamingChunkPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CoolstreamingChunkPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int CoolstreamingChunkPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *CoolstreamingChunkPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "sequenceNumber",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int CoolstreamingChunkPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumber")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CoolstreamingChunkPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *CoolstreamingChunkPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"getter")) return "getSequenceNumber";
            if (!strcmp(propertyname,"setter")) return "setSequenceNumber";
            return NULL;
        default: return NULL;
    }
}

int CoolstreamingChunkPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingChunkPacket *pp = (CoolstreamingChunkPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CoolstreamingChunkPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingChunkPacket *pp = (CoolstreamingChunkPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSequenceNumber());
        default: return "";
    }
}

bool CoolstreamingChunkPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingChunkPacket *pp = (CoolstreamingChunkPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setSequenceNumber(string2long(value)); return true;
        default: return false;
    }
}

const char *CoolstreamingChunkPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *CoolstreamingChunkPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CoolstreamingChunkPacket *pp = (CoolstreamingChunkPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



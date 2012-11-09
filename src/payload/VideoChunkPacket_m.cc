//
// Generated file, do not edit! Created by opp_msgc 4.2 from applications/common/messages/VideoChunkPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "VideoChunkPacket_m.h"

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




Register_Class(VideoChunkPacket);

VideoChunkPacket::VideoChunkPacket(const char *name, int kind) : PeerStreamingPacket(name,kind)
{
    this->setPacketGroup(PACKET_GROUP_VIDEO_CHUNK);

    this->seqNumber_var = 0;
    this->timeStamp_var = 0;
}

VideoChunkPacket::VideoChunkPacket(const VideoChunkPacket& other) : PeerStreamingPacket(other)
{
    copy(other);
}

VideoChunkPacket::~VideoChunkPacket()
{
}

VideoChunkPacket& VideoChunkPacket::operator=(const VideoChunkPacket& other)
{
    if (this==&other) return *this;
    PeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void VideoChunkPacket::copy(const VideoChunkPacket& other)
{
    this->seqNumber_var = other.seqNumber_var;
    this->timeStamp_var = other.timeStamp_var;
}

void VideoChunkPacket::parsimPack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimPack(b);
    doPacking(b,this->seqNumber_var);
    doPacking(b,this->timeStamp_var);
}

void VideoChunkPacket::parsimUnpack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->seqNumber_var);
    doUnpacking(b,this->timeStamp_var);
}

long VideoChunkPacket::getSeqNumber() const
{
    return seqNumber_var;
}

void VideoChunkPacket::setSeqNumber(long seqNumber)
{
    this->seqNumber_var = seqNumber;
}

double VideoChunkPacket::getTimeStamp() const
{
    return timeStamp_var;
}

void VideoChunkPacket::setTimeStamp(double timeStamp)
{
    this->timeStamp_var = timeStamp;
}

class VideoChunkPacketDescriptor : public cClassDescriptor
{
  public:
    VideoChunkPacketDescriptor();
    virtual ~VideoChunkPacketDescriptor();

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

Register_ClassDescriptor(VideoChunkPacketDescriptor);

VideoChunkPacketDescriptor::VideoChunkPacketDescriptor() : cClassDescriptor("VideoChunkPacket", "PeerStreamingPacket")
{
}

VideoChunkPacketDescriptor::~VideoChunkPacketDescriptor()
{
}

bool VideoChunkPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<VideoChunkPacket *>(obj)!=NULL;
}

const char *VideoChunkPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int VideoChunkPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int VideoChunkPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *VideoChunkPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "seqNumber",
        "timeStamp",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int VideoChunkPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqNumber")==0) return base+0;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeStamp")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *VideoChunkPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "long",
        "double",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *VideoChunkPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"getter")) return "getSeqNumber";
            if (!strcmp(propertyname,"setter")) return "setSeqNumber";
            return NULL;
        case 1:
            if (!strcmp(propertyname,"getter")) return "getTimeStamp";
            if (!strcmp(propertyname,"setter")) return "setTimeStamp";
            return NULL;
        default: return NULL;
    }
}

int VideoChunkPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    VideoChunkPacket *pp = (VideoChunkPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string VideoChunkPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    VideoChunkPacket *pp = (VideoChunkPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSeqNumber());
        case 1: return double2string(pp->getTimeStamp());
        default: return "";
    }
}

bool VideoChunkPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    VideoChunkPacket *pp = (VideoChunkPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setSeqNumber(string2long(value)); return true;
        case 1: pp->setTimeStamp(string2double(value)); return true;
        default: return false;
    }
}

const char *VideoChunkPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *VideoChunkPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    VideoChunkPacket *pp = (VideoChunkPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



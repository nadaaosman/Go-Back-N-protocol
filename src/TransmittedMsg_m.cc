//
// Generated file, do not edit! Created by nedtool 5.6 from TransmittedMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "TransmittedMsg_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

TransmittedMsg_Base::TransmittedMsg_Base(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->parity = 0;
    this->seqNumber = 0;
    this->frameType = 0;
    this->ackNumber = 0;
    this->time = 0;
}

TransmittedMsg_Base::TransmittedMsg_Base(const TransmittedMsg_Base& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

TransmittedMsg_Base::~TransmittedMsg_Base()
{
}

TransmittedMsg_Base& TransmittedMsg_Base::operator=(const TransmittedMsg_Base& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void TransmittedMsg_Base::copy(const TransmittedMsg_Base& other)
{
    this->parity = other.parity;
    this->seqNumber = other.seqNumber;
    this->payload = other.payload;
    this->frameType = other.frameType;
    this->ackNumber = other.ackNumber;
    this->time = other.time;
    this->Error = other.Error;
}

void TransmittedMsg_Base::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->parity);
    doParsimPacking(b,this->seqNumber);
    doParsimPacking(b,this->payload);
    doParsimPacking(b,this->frameType);
    doParsimPacking(b,this->ackNumber);
    doParsimPacking(b,this->time);
    doParsimPacking(b,this->Error);
}

void TransmittedMsg_Base::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->parity);
    doParsimUnpacking(b,this->seqNumber);
    doParsimUnpacking(b,this->payload);
    doParsimUnpacking(b,this->frameType);
    doParsimUnpacking(b,this->ackNumber);
    doParsimUnpacking(b,this->time);
    doParsimUnpacking(b,this->Error);
}

int TransmittedMsg_Base::getParity() const
{
    return this->parity;
}

void TransmittedMsg_Base::setParity(int parity)
{
    this->parity = parity;
}

int TransmittedMsg_Base::getSeqNumber() const
{
    return this->seqNumber;
}

void TransmittedMsg_Base::setSeqNumber(int seqNumber)
{
    this->seqNumber = seqNumber;
}

const char * TransmittedMsg_Base::getPayload() const
{
    return this->payload.c_str();
}

void TransmittedMsg_Base::setPayload(const char * payload)
{
    this->payload = payload;
}

int TransmittedMsg_Base::getFrameType() const
{
    return this->frameType;
}

void TransmittedMsg_Base::setFrameType(int frameType)
{
    this->frameType = frameType;
}

int TransmittedMsg_Base::getAckNumber() const
{
    return this->ackNumber;
}

void TransmittedMsg_Base::setAckNumber(int ackNumber)
{
    this->ackNumber = ackNumber;
}

double TransmittedMsg_Base::getTime() const
{
    return this->time;
}

void TransmittedMsg_Base::setTime(double time)
{
    this->time = time;
}

const char * TransmittedMsg_Base::getError() const
{
    return this->Error.c_str();
}

void TransmittedMsg_Base::setError(const char * Error)
{
    this->Error = Error;
}

class TransmittedMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    TransmittedMsgDescriptor();
    virtual ~TransmittedMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(TransmittedMsgDescriptor)

TransmittedMsgDescriptor::TransmittedMsgDescriptor() : omnetpp::cClassDescriptor("TransmittedMsg", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

TransmittedMsgDescriptor::~TransmittedMsgDescriptor()
{
    delete[] propertynames;
}

bool TransmittedMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<TransmittedMsg_Base *>(obj)!=nullptr;
}

const char **TransmittedMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = { "customize",  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *TransmittedMsgDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int TransmittedMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount() : 7;
}

unsigned int TransmittedMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *TransmittedMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "parity",
        "seqNumber",
        "payload",
        "frameType",
        "ackNumber",
        "time",
        "Error",
    };
    return (field>=0 && field<7) ? fieldNames[field] : nullptr;
}

int TransmittedMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "parity")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqNumber")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "payload")==0) return base+2;
    if (fieldName[0]=='f' && strcmp(fieldName, "frameType")==0) return base+3;
    if (fieldName[0]=='a' && strcmp(fieldName, "ackNumber")==0) return base+4;
    if (fieldName[0]=='t' && strcmp(fieldName, "time")==0) return base+5;
    if (fieldName[0]=='E' && strcmp(fieldName, "Error")==0) return base+6;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *TransmittedMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "string",
        "int",
        "int",
        "double",
        "string",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : nullptr;
}

const char **TransmittedMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *TransmittedMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int TransmittedMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    TransmittedMsg_Base *pp = (TransmittedMsg_Base *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *TransmittedMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    TransmittedMsg_Base *pp = (TransmittedMsg_Base *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string TransmittedMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    TransmittedMsg_Base *pp = (TransmittedMsg_Base *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getParity());
        case 1: return long2string(pp->getSeqNumber());
        case 2: return oppstring2string(pp->getPayload());
        case 3: return long2string(pp->getFrameType());
        case 4: return long2string(pp->getAckNumber());
        case 5: return double2string(pp->getTime());
        case 6: return oppstring2string(pp->getError());
        default: return "";
    }
}

bool TransmittedMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    TransmittedMsg_Base *pp = (TransmittedMsg_Base *)object; (void)pp;
    switch (field) {
        case 0: pp->setParity(string2long(value)); return true;
        case 1: pp->setSeqNumber(string2long(value)); return true;
        case 2: pp->setPayload((value)); return true;
        case 3: pp->setFrameType(string2long(value)); return true;
        case 4: pp->setAckNumber(string2long(value)); return true;
        case 5: pp->setTime(string2double(value)); return true;
        case 6: pp->setError((value)); return true;
        default: return false;
    }
}

const char *TransmittedMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *TransmittedMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    TransmittedMsg_Base *pp = (TransmittedMsg_Base *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}



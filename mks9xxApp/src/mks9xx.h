
/*
* mks9xx.h
*
*/

#ifndef mks9xx_H_
#define mks9xx_H_

#include <epicsTypes.h>
#include <epicsThread.h>
#include <asynPortDriver.h>
#include "protocol.h"

//*!*Section messageDeclaration begin*!*
class MsgFailReply: public Message
{
public:
    MsgFailReply();
    ConstStr* pre;
    TextInt<epicsInt32>* val;
    ConstStr* post;
};
class MsgStringReply: public Message
{
public:
    MsgStringReply();
    ConstStr* pre;
    TerminatedStr* val;
};
class MsgFloatReply: public Message
{
public:
    MsgFloatReply();
    ConstStr* pre;
    TextFloat<epicsFloat64>* val;
    ConstStr* post;
};
class MsgGetAnOutputFormat: public Message
{
public:
    MsgGetAnOutputFormat();
    ConstStr* pre;
};
class MsgSetAnOutputFormat: public Message
{
public:
    MsgSetAnOutputFormat();
    ConstStr* pre;
    TerminatedEnum* val;
};
class MsgReplyAnOutputFormat: public Message
{
public:
    MsgReplyAnOutputFormat();
    ConstStr* pre;
    TerminatedEnum* val;
};
class MsgGetUnit: public Message
{
public:
    MsgGetUnit();
    ConstStr* pre;
};
class MsgSetUnit: public Message
{
public:
    MsgSetUnit();
    ConstStr* pre;
    TerminatedEnum* val;
};
class MsgReplyUnit: public Message
{
public:
    MsgReplyUnit();
    ConstStr* pre;
    TerminatedEnum* val;
};
class MsgGetUserTag: public Message
{
public:
    MsgGetUserTag();
    ConstStr* pre;
};
class MsgSetUsertag: public Message
{
public:
    MsgSetUsertag();
    ConstStr* pre;
    TerminatedStr* val;
};
class MsgGetTransducerType: public Message
{
public:
    MsgGetTransducerType();
    ConstStr* pre;
};
class MsgGetFirmwareVersion: public Message
{
public:
    MsgGetFirmwareVersion();
    ConstStr* pre;
};
class MsgGetHardwareVersion: public Message
{
public:
    MsgGetHardwareVersion();
    ConstStr* pre;
};
class MsgGetManufacturer: public Message
{
public:
    MsgGetManufacturer();
    ConstStr* pre;
};
class MsgGetModel: public Message
{
public:
    MsgGetModel();
    ConstStr* pre;
};
class MsgGetPressure: public Message
{
public:
    MsgGetPressure();
    ConstStr* pre;
};
class MsgGetSerialNumber: public Message
{
public:
    MsgGetSerialNumber();
    ConstStr* pre;
};
class MsgGetTimeOn: public Message
{
public:
    MsgGetTimeOn();
    ConstStr* pre;
};
class MsgGetTransducerTemperature: public Message
{
public:
    MsgGetTransducerTemperature();
    ConstStr* pre;
};
class MsgGetSetPoint: public Message
{
public:
    MsgGetSetPoint();
    ConstStr* pre;
};
class MsgSetSetPoint: public Message
{
public:
    MsgSetSetPoint();
    ConstStr* pre;
    TextFloat<epicsFloat64>* val;
    ConstStr* post;
};
class MsgGetHysteresis: public Message
{
public:
    MsgGetHysteresis();
    ConstStr* pre;
};
class MsgSetHysteresis: public Message
{
public:
    MsgSetHysteresis();
    ConstStr* pre;
    TextFloat<epicsFloat64>* val;
    ConstStr* post;
};
class MsgGetSpDirection: public Message
{
public:
    MsgGetSpDirection();
    ConstStr* pre;
};
class MsgSetSpDirection: public Message
{
public:
    MsgSetSpDirection();
    ConstStr* pre;
    TerminatedEnum* val;
};
class MsgReplySpDirection: public Message
{
public:
    MsgReplySpDirection();
    ConstStr* pre;
    TerminatedEnum* val;
};
class MsgGetSpEnable: public Message
{
public:
    MsgGetSpEnable();
    ConstStr* pre;
};
class MsgSetSpEnable: public Message
{
public:
    MsgSetSpEnable();
    ConstStr* pre;
    TerminatedEnum* val;
};
class MsgReplySpEnable: public Message
{
public:
    MsgReplySpEnable();
    ConstStr* pre;
    TerminatedEnum* val;
};
class MsgGetSpStatus: public Message
{
public:
    MsgGetSpStatus();
    ConstStr* pre;
};
class MsgReplySpStatus: public Message
{
public:
    MsgReplySpStatus();
    ConstStr* pre;
    TerminatedEnum* val;
};
//*!*Section messageDeclaration end*!*

class mks9xx : public asynPortDriver
{
// Read thread class
public:
    class ReadThread: public epicsThreadRunable
    {
    private:
        mks9xx* owner;
        epicsThread thread;
    public:
        ReadThread(mks9xx* owner);
        virtual ~ReadThread() {}
        virtual void run() {owner->readRun();}
    };
// Construction
public:
    mks9xx(const char* portName,
        const char* serialPortName, int serialPortAddress);
    virtual ~mks9xx();
// These are the methods that we override from asynPortDriver
public:
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
    virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual);
// This is the method called by the read thread to perform its function
public:
    void readRun();
// Data
private:
    //*!*Section enumDefinition begin*!*
    enum {Connection_Disconnected=0, Connection_Connected};
    enum {AnalogueOutputFormat_Log5=0, AnalogueOutputFormat_Log10, AnalogueOutputFormat_Linear5, AnalogueOutputFormat_Linear10};
    enum {Units_Torr=0, Units_mBar, Units_Pascal};
    enum {SetPointDirection_Below=0, SetPointDirection_Above};
    enum {SetPointEnable_Disable=0, SetPointEnable_Enable};
    enum {SetPointState_Clear=0, SetPointState_Set};
    //*!*Section enumDefinition end*!*
    //*!*Section stringDeclaration begin*!*
    static const char* nameConnection;
    static const char* nameAnalogueOutputFormat;
    static const char* nameUnits;
    static const char* nameUserTag;
    static const char* nameTransducerType;
    static const char* nameFirmwareVersion;
    static const char* nameHardwareVersion;
    static const char* nameManufacturer;
    static const char* nameModel;
    static const char* namePressure;
    static const char* nameSerialNumber;
    static const char* nameTimeOn;
    static const char* nameTransducerTemperature;
    static const char* nameSetPoint;
    static const char* nameSetPointHysteresis;
    static const char* nameSetPointDirection;
    static const char* nameSetPointEnable;
    static const char* nameSetPointState;
    //*!*Section stringDeclaration end*!*
    int FIRST_PARAM;
    //*!*Section indexDeclaration begin*!*
    int indexConnection;
    int indexAnalogueOutputFormat;
    int indexUnits;
    int indexUserTag;
    int indexTransducerType;
    int indexFirmwareVersion;
    int indexHardwareVersion;
    int indexManufacturer;
    int indexModel;
    int indexPressure;
    int indexSerialNumber;
    int indexTimeOn;
    int indexTransducerTemperature;
    int indexSetPoint;
    int indexSetPointHysteresis;
    int indexSetPointDirection;
    int indexSetPointEnable;
    int indexSetPointState;
    //*!*Section indexDeclaration end*!*
    int LAST_PARAM;
    //*!*Section protocolDeclaration begin*!*
    Protocol* protocolAnOutputFormat;
    Protocol* protocolUnit;
    Protocol* protocolUserTag;
    Protocol* protocolTransducerType;
    Protocol* protocolFirmwareVersion;
    Protocol* protocolHardwareVersion;
    Protocol* protocolManufacturer;
    Protocol* protocolModel;
    Protocol* protocolPressure;
    Protocol* protocolSerialNumber;
    Protocol* protocolTimeOn;
    Protocol* protocolTransducerTemperature;
    Protocol* protocolSetPoint;
    Protocol* protocolHysteresis;
    Protocol* protocolSpDirection;
    Protocol* protocolSpEnable;
    Protocol* protocolSpStatus;
    //*!*Section protocolDeclaration end*!*
    asynUser* serialPortUser;
    ReadThread* readIt;
    static const double pollPeriod;
    static const int userTagLength;
    MsgGetTransducerType msgTransducerType;
    MsgGetFirmwareVersion msgFirmwareVersion;
    MsgGetHardwareVersion msgHardwareVersion;
    MsgGetManufacturer msgManufacturer;
    MsgGetModel msgModel;
    MsgGetSerialNumber msgSerialNumber;
    MsgSetAnOutputFormat msgAnOutputFormat;
    MsgSetHysteresis msgHysteresis;
    MsgSetSetPoint msgSetPoint;
    MsgSetSpDirection msgSpDirection;
    MsgSetSpEnable msgSpEnable;
    MsgSetUnit msgUnit;
    MsgSetUsertag msgSetUserTag;
    MsgGetUserTag msgUserTag;
    MsgGetPressure msgPressure;
    MsgGetTimeOn msgTimeOn;
    MsgGetTransducerTemperature msgTransducerTemperature;
    MsgGetSpStatus msgSpStatus;
// Helper functions
private:
    bool getString(Protocol* protocol, Message* cmd, int handle);
    bool getFloat(Protocol* protocol, Message* cmd, int handle);
    bool getSpStatus(Protocol* protocol, Message* cmd, int handle);
    bool setInt(Protocol* protocol, Message* cmd, TerminatedEnum* valField, int handle);
    bool setFloat(Protocol* protocol, Message* cmd, TextFloat<epicsFloat64>* valField, int handle);
    bool setString(Protocol* protocol, Message* cmd, TerminatedStr* valField, int handle);
};

#endif

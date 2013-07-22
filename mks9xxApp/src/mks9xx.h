
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
class MsgIntReply: public Message
	{
	public:
		MsgIntReply();
		ConstStr* pre;
		TextInt<epicsInt32>* val;
		ConstStr* post;
	};
class MsgGetAnOutputFormat: public Message
{
public:
    MsgGetAnOutputFormat(int channel_in = 1);
    ConstStr* pre;
	TextInt<epicsInt32>* channel;
    ConstStr* post;
};
class MsgSetAnOutputFormat: public Message
{
public:
    MsgSetAnOutputFormat(int channel_in = 1);
    ConstStr* pre;
	TextInt<epicsInt32>* channel;
	ConstStr* pling;
    TerminatedEnum* val;
    ConstStr* post;
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
class MsgGetUnitReply: public Message
{
public:
    MsgGetUnitReply();
    ConstStr* pre;
	TerminatedEnum* val;
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
		MsgGetPressure(int gauge_in = 1);
		ConstStr* pre;
		TextInt<epicsInt32>* gauge;
		ConstStr* post;
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
class MsgGetSetpoint: public Message
	{
	public:
		MsgGetSetpoint(int relay_in = 1);
		ConstStr* pre;
		TextInt<epicsInt32>* relay;
		ConstStr* post;
	};
class MsgSetSetpoint: public Message
	{
	public:
		MsgSetSetpoint(int relay_in = 1);

		ConstStr* pre;
		TextInt<epicsInt32>* relay;
		ConstStr* pling;
		TextFloat<epicsFloat64>* val;
		ConstStr* post;
	};
class MsgGetHysteresis: public Message
{
public:
    MsgGetHysteresis(int relay_in = 1);
    ConstStr* pre;
	TextInt<epicsInt32>* relay;
	ConstStr* post;
};
class MsgSetHysteresis: public Message
{
public:
    MsgSetHysteresis(int relay_in = 1);
    ConstStr* pre;
	TextInt<epicsInt32>* relay;
	ConstStr* pling;
    TextFloat<epicsFloat64>* val;
    ConstStr* post;
};
class MsgGetSpDirection: public Message
{
public:
    MsgGetSpDirection(int relay_in = 1);
    ConstStr* pre;
	TextInt<epicsInt32>* relay;
	ConstStr* post;
};
class MsgSetSpDirection: public Message
{
public:
    MsgSetSpDirection(int relay_in = 1);
    ConstStr* pre;
	TextInt<epicsInt32>* relay;
	ConstStr* pling;
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
    MsgGetSpEnable(int relay_in = 1);
    ConstStr* pre;
	TextInt<epicsInt32>* relay;
	ConstStr* post;
};
class MsgSetSpEnable: public Message
{
public:
    MsgSetSpEnable(int relay_in = 1);
    ConstStr* pre;
	TextInt<epicsInt32>* relay;
	ConstStr* pling;
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
		MsgGetSpStatus(int relay_in = 1);
		ConstStr* pre;
		TextInt<epicsInt32>* relay;
		ConstStr* post;
	};
class MsgReplySpStatus: public Message
{
public:
    MsgReplySpStatus();
    ConstStr* pre;
    TerminatedEnum* val;
};

class MsgGetTransducerType: public Message
	{
	public:
		MsgGetTransducerType();
		ConstStr* pre;
	};
class MsgReplyTransducerType: public Message
	{
	public:
		MsgReplyTransducerType();
		ConstStr* pre;
		TerminatedEnum* val;
	};
class MsgGetTransducerStatus: public Message
	{
	public:
		MsgGetTransducerStatus();
		ConstStr* pre;
	};
class MsgReplyTransducerStatus: public Message
	{
	public:
		MsgReplyTransducerStatus();
		ConstStr* pre;
	    TerminatedEnum* val;
	};

class MsgSetCCControl: public Message
{
public:
    MsgSetCCControl();
    ConstStr* pre;
    TerminatedEnum* val;
};
class MsgReplyCCControl: public Message
{
public:
    MsgReplyCCControl();
    ConstStr* pre;
    TerminatedEnum* val;
};

class MsgSetGasType: public Message
{
public:
    MsgSetGasType();
    ConstStr* pre;
    TerminatedEnum* val;
};
class MsgGetGasType: public Message
{
public:
    MsgGetGasType();
    ConstStr* pre;
};
class MsgReplyGasType: public Message
{
public:
    MsgReplyGasType();
    ConstStr* pre;
    TerminatedEnum* val;
};

class MsgSetLock: public Message
	{
	public:
		MsgSetLock();
		ConstStr* pre;
		TerminatedEnum* val;
	};
class MsgGetLock: public Message
	{
	public:
		MsgGetLock();
		ConstStr* pre;
	};
class MsgReplyLock: public Message
	{
	public:
		MsgReplyLock();
		ConstStr* pre;
		TerminatedEnum* val;
	};
class MsgGetCCPressureDose: public Message
	{
	public:
		MsgGetCCPressureDose();
		ConstStr* pre;
	};


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

    enum {Connection_Disconnected=0, Connection_Connected};
    enum {AnalogueOutputFormat_Log5=0, AnalogueOutputFormat_Log10, AnalogueOutputFormat_Linear5, AnalogueOutputFormat_Linear10};
    enum {Units_Torr=0, Units_mBar, Units_Pascal};
    enum {SetPointDirection_Below=0, SetPointDirection_Above};
    enum {SetPointEnable_Disable=0, SetPointEnable_Enable};
    enum {SetPointState_Clear=0, SetPointState_Set};
    enum {CCState_Off=0, CCState_On};
    enum {TT_UNDEFINED=-1, TT_LOADLOCK=0, TT_DUALTRANS, TT_MICROPIRANI, TT_UNIMAG, TT_DUALMAG, TT_QUADMAG}; // Transducer types

    static const int NUM_RELAYS  = 3; // Setpoint relays
    static const int NUM_SENSORS = 5; // Pressure sensors
    static const int NUM_AOCHANS = 2; // Analogue output channels

    static const char* nameConnection;
    static const char* nameAnalogueOutputFormat1;
    static const char* nameAnalogueOutputFormat2;
    static const char* nameUnits;
    static const char* nameUserTag;
    static const char* nameTransducerType;
    static const char* nameFirmwareVersion;
    static const char* nameHardwareVersion;
    static const char* nameManufacturer;
    static const char* nameModel;
    static const char* namePressure1;
    static const char* namePressure2;
    static const char* namePressure3;
    static const char* namePressure4;
    static const char* namePressure5;
    static const char* nameSerialNumber;
    static const char* nameTimeOn;
    static const char* nameTransducerTemperature;
    static const char* nameSetPoint1;
    static const char* nameSetPoint2;
    static const char* nameSetPoint3;
    static const char* nameSetPointHysteresis1;
    static const char* nameSetPointHysteresis2;
    static const char* nameSetPointHysteresis3;
    static const char* nameSetPointDirection1;
    static const char* nameSetPointDirection2;
    static const char* nameSetPointDirection3;
    static const char* nameSetPointEnable1;
    static const char* nameSetPointEnable2;
    static const char* nameSetPointEnable3;
    static const char* nameSetPointState1;
    static const char* nameSetPointState2;
    static const char* nameSetPointState3;
    static const char* nameTransducerStatus;
    static const char* nameCCControl;
    static const char* nameGasType;
    static const char* nameLock;
    static const char* nameCCPressureDose;

    int FIRST_PARAM;

    int indexConnection;
    int indexAnalogueOutputFormat1;
    int indexAnalogueOutputFormat2;
    int indexUnits;
    int indexUserTag;
    int indexTransducerType;
    int indexFirmwareVersion;
    int indexHardwareVersion;
    int indexManufacturer;
    int indexModel;
    int indexPressure1;
    int indexPressure2;
    int indexPressure3;
    int indexPressure4;
    int indexPressure5;
    int indexSerialNumber;
    int indexTimeOn;
    int indexTransducerTemperature;
    int indexSetPoint1;
    int indexSetPoint2;
    int indexSetPoint3;
    int indexSetPointHysteresis1;
    int indexSetPointHysteresis2;
    int indexSetPointHysteresis3;
    int indexSetPointDirection1;
    int indexSetPointDirection2;
    int indexSetPointDirection3;
    int indexSetPointEnable1;
    int indexSetPointEnable2;
    int indexSetPointEnable3;
    int indexSetPointState1;
    int indexSetPointState2;
    int indexSetPointState3;
    int indexTransducerStatus;
    int indexCCControl;
    int indexGasType;
    int indexLock;
    int indexCCPressureDose;

    int LAST_PARAM;

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
    Protocol* protocolTransducerStatus;
    Protocol* protocolCCControl;
    Protocol* protocolGasType;
    Protocol* protocolLock;
    Protocol* protocolCCPressureDose;

    asynUser* 					serialPortUser;
    ReadThread* 				readIt;
    static const double 		pollPeriod;
    static const int 			userTagLength;
    MsgGetFirmwareVersion 		msgFirmwareVersion;
    MsgGetHardwareVersion 		msgHardwareVersion;
    MsgGetManufacturer 			msgManufacturer;
    MsgGetModel 				msgModel;
    MsgGetSerialNumber 			msgSerialNumber;
    MsgSetAnOutputFormat 		*msgSetAnOutputFormat[NUM_AOCHANS];
    MsgGetAnOutputFormat 		*msgGetAnOutputFormat[NUM_AOCHANS];
    MsgGetHysteresis 			*msgGetHysteresis[NUM_RELAYS];
    MsgSetHysteresis 			*msgSetHysteresis[NUM_RELAYS];
    MsgSetSetpoint 				*msgSetSetpoint[NUM_RELAYS];
    MsgGetSetpoint 				*msgGetSetpoint[NUM_RELAYS];
    MsgGetSpDirection 			*msgGetSpDirection[NUM_RELAYS];
    MsgSetSpDirection 			*msgSetSpDirection[NUM_RELAYS];
    MsgGetSpEnable 				*msgGetSpEnable[NUM_RELAYS];
    MsgSetSpEnable 				*msgSetSpEnable[NUM_RELAYS];
    MsgSetUnit 					msgSetUnit;
    MsgGetUnit 					msgGetUnit;
    MsgSetUsertag 				msgSetUserTag;
    MsgGetUserTag 				msgUserTag;
    MsgGetPressure 				*msgPressure[NUM_SENSORS];
    MsgGetTimeOn 				msgTimeOn;
    MsgGetTransducerTemperature msgTransducerTemperature;
    MsgGetSpStatus 				*msgGetSpStatus[NUM_RELAYS];
    MsgGetTransducerType		msgTransducerType;
    MsgGetTransducerStatus 		msgTransducerStatus;
    MsgSetCCControl     		msgCCControl;
    MsgSetGasType 	    		msgSetGasType;
    MsgGetGasType 	    		msgGetGasType;
    MsgSetLock 	    			msgSetLock;
    MsgGetLock 	    			msgGetLock;
    MsgGetCCPressureDose		msgGetCCPressureDose;

    // Gauge one-time descriptors
    int							transducerType;


    // Helper functions
private:
    bool getString(Protocol* protocol, Message* cmd, int handle);
    bool getFloat(Protocol* protocol, Message* cmd, int handle);
    bool getInt(Protocol* protocol, Message* cmd, int handle);
    bool getSpStatus(Protocol* protocol, Message* cmd, int handle);
    bool getTransducerType(Protocol* protocol, Message* cmd, int handle);
    bool getTransducerStatus(Protocol* protocol, Message* cmd, int handle);
    bool setInt(Protocol* protocol, Message* cmd, TerminatedEnum* valField, int handle);
    bool setInt(Protocol* protocol, Message* cmd, TextInt<epicsInt32>* valField, int handle);
    bool setFloat(Protocol* protocol, Message* cmd, TextFloat<epicsFloat64>* valField, int handle);
    bool setString(Protocol* protocol, Message* cmd, TerminatedStr* valField, int handle);
    bool getUnit(Protocol* protocol, Message* cmd, int handle);
    bool getGasType(Protocol* protocol, Message* cmd, int handle);
    bool getSpEnable(Protocol* protocol, Message* cmd, int handle);
    bool getSpDirection(Protocol* protocol, Message* cmd, int handle);
    bool getAnOutputFormat(Protocol* protocol, Message* cmd, int handle);
};

#endif

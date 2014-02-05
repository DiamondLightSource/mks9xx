
/*
* mks9xx.cpp
*
*/

#include "mks9xx.h"
#include <asynOctetSyncIO.h>
#include <iocsh.h>
#include <epicsExport.h>

// Parameter name definitions
const char* mks9xx::nameConnection            = "CONNECTION";
const char* mks9xx::nameAnalogueOutputFormat1 = "ANALOGUEOUTPUTFORMAT1";
const char* mks9xx::nameAnalogueOutputFormat2 = "ANALOGUEOUTPUTFORMAT2";
const char* mks9xx::nameUnits                 = "UNITS";
const char* mks9xx::nameUserTag               = "USERTAG";
const char* mks9xx::nameFirmwareVersion       = "FIRMWAREVERSION";
const char* mks9xx::nameHardwareVersion       = "HARDWAREVERSION";
const char* mks9xx::nameManufacturer          = "MANUFACTURER";
const char* mks9xx::nameModel                 = "MODEL";
const char* mks9xx::namePressure1             = "PRESSURE1";
const char* mks9xx::namePressure2             = "PRESSURE2";
const char* mks9xx::namePressure3             = "PRESSURE3";
const char* mks9xx::namePressure4             = "PRESSURE4";
const char* mks9xx::namePressure5             = "PRESSURE5";
const char* mks9xx::nameSerialNumber          = "SERIALNUMBER";
const char* mks9xx::nameTimeOn                = "TIMEON";
const char* mks9xx::nameTransducerTemperature = "TRANSDUCERTEMPERATURE";
const char* mks9xx::nameSetPoint1             = "SETPOINT1";
const char* mks9xx::nameSetPoint2             = "SETPOINT2";
const char* mks9xx::nameSetPoint3             = "SETPOINT3";
const char* mks9xx::nameSetPointHysteresis1   = "SETPOINTHYSTERESIS1";
const char* mks9xx::nameSetPointHysteresis2   = "SETPOINTHYSTERESIS2";
const char* mks9xx::nameSetPointHysteresis3   = "SETPOINTHYSTERESIS3";
const char* mks9xx::nameSetPointDirection1    = "SETPOINTDIRECTION1";
const char* mks9xx::nameSetPointDirection2    = "SETPOINTDIRECTION2";
const char* mks9xx::nameSetPointDirection3    = "SETPOINTDIRECTION3";
const char* mks9xx::nameSetPointEnable1       = "SETPOINTENABLE1";
const char* mks9xx::nameSetPointEnable2       = "SETPOINTENABLE2";
const char* mks9xx::nameSetPointEnable3       = "SETPOINTENABLE3";
const char* mks9xx::nameSetPointState1        = "SETPOINTSTATE1";
const char* mks9xx::nameSetPointState2        = "SETPOINTSTATE2";
const char* mks9xx::nameSetPointState3        = "SETPOINTSTATE3";
const char* mks9xx::nameTransducerType        = "TRANSDUCERTYPE";
const char* mks9xx::nameTransducerStatus      = "TRANSDUCERSTATUS";
const char* mks9xx::nameCCControl             = "CCCON";
const char* mks9xx::nameGasType               = "GASTYPE";
const char* mks9xx::nameLock                  = "LOCK";
const char* mks9xx::nameCCPressureDose        = "CCDOSE";
const char* mks9xx::nameSTA				      = "STA";

#define NUM_PARAMS (&LAST_PARAM - &FIRST_PARAM - 1)
const double mks9xx::pollPeriod = 1.0;
const int mks9xx::userTagLength = 30;

MsgFailReply::MsgFailReply()
    : Message("MsgFailReply")
	{
    pre = new ConstStr("pre", this, "@253NAK", 7);
    val = new TextInt<epicsInt32>("val", this, /*base=*/10);
    post = new ConstStr("post", this, ";FF", 3);
	}
MsgStringReply::MsgStringReply()
    : Message("MsgStringReply")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedStr("val", this, ";FF");
	}
MsgFloatReply::MsgFloatReply()
    : Message("MsgFloatReply")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
//    val = new TextFloat<epicsFloat64>("val", this, 1, false);
    val = new TextFloat<epicsFloat64>("val", this, 2, false);
    post = new ConstStr("post", this, ";FF", 3);
	}
MsgIntReply::MsgIntReply()
    : Message("MsgIntReply")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TextInt<epicsInt32>("val", this, /*base=*/10);
    post = new ConstStr("post", this, ";FF", 3);
	}
MsgGetAnOutputFormat::MsgGetAnOutputFormat(int channel_in)
    : Message("MsgGetAnOutputFormat")
	{
    pre = new ConstStr("pre", this, "@253AO", 6);
    channel = new TextInt<epicsInt32>("channel", this, /*base=*/10);
    post = new ConstStr("post", this, "?;FF", 4);
	*channel = channel_in;
	}
MsgSetAnOutputFormat::MsgSetAnOutputFormat(int channel_in)
    : Message("MsgSetAnOutputFormat")
	{
    pre = new ConstStr("pre", this, "@253AO", 6);
    channel = new TextInt<epicsInt32>("channel", this, /*base=*/10);
    pling = new ConstStr("pling", this, "!", 1);
    val = new TerminatedEnum("val", this, ";FF", "10""\0""20""\0""30\0\0");
    *channel = channel_in;
	}

MsgReplyAnOutputFormat::MsgReplyAnOutputFormat()
    : Message("MsgReplyAnOutputFormat")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "10""\0""20""\0""30\0\0");
	}

MsgGetUnit::MsgGetUnit()
    : Message("MsgGetUnit")
	{
    pre = new ConstStr("pre", this, "@253U?;FF", 9);
	}
MsgSetUnit::MsgSetUnit()
    : Message("MsgSetUnit")
	{
    pre = new ConstStr("pre", this, "@253U!", 6);
    val = new TerminatedEnum("val", this, ";FF", "TORR\0MBAR\0PASCAL\0\0");
	}
MsgReplyUnit::MsgReplyUnit()
    : Message("MsgReplyUnit")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "TORR\0MBAR\0PASCAL\0\0");
	}
MsgGetUserTag::MsgGetUserTag()
    : Message("MsgGetUserTag")
	{
    pre = new ConstStr("pre", this, "@253UT?;FF", 10);
	}
MsgSetUsertag::MsgSetUsertag()
    : Message("MsgSetUsertag")
	{
    pre = new ConstStr("pre", this, "@253UT!", 7);
    val = new TerminatedStr("val", this, ";FF");
	}
MsgGetFirmwareVersion::MsgGetFirmwareVersion()
    : Message("MsgGetFirmwareVersion")
	{
    pre = new ConstStr("pre", this, "@253FV?;FF", 10);
	}
MsgGetHardwareVersion::MsgGetHardwareVersion()
    : Message("MsgGetHardwareVersion")
	{
    pre = new ConstStr("pre", this, "@253HV?;FF", 10);
	}
MsgGetManufacturer::MsgGetManufacturer()
    : Message("MsgGetManufacturer")
	{
    pre = new ConstStr("pre", this, "@253MF?;FF", 10);
	}
MsgGetModel::MsgGetModel()
    : Message("MsgGetModel")
	{
    pre = new ConstStr("pre", this, "@253MD?;FF", 10);
	}
MsgGetPressure::MsgGetPressure(int gauge_in)
    : Message("MsgGetPressure")
	{
    pre = new ConstStr("pre", this, "@253PR", 6);
    gauge = new TextInt<epicsInt32>("gauge", this, /*base=*/10);
    post = new ConstStr("post", this, "?;FF", 4);
	*gauge = gauge_in;
	}
MsgGetSerialNumber::MsgGetSerialNumber()
    : Message("MsgGetSerialNumber")
	{
    pre = new ConstStr("pre", this, "@253SN?;FF", 10);
	}
MsgGetTimeOn::MsgGetTimeOn()
    : Message("MsgGetTimeOn")
	{
    pre = new ConstStr("pre", this, "@253TIM?;FF", 11);
	}
MsgGetTransducerTemperature::MsgGetTransducerTemperature()
    : Message("MsgGetTransducerTemperature")
	{
    pre = new ConstStr("pre", this, "@253TEM?;FF", 11);
	}
MsgGetSetpoint::MsgGetSetpoint(int relay_in)
    : Message("MsgGetSetpoint")
	{
    pre = new ConstStr("pre", this, "@253SP", 6);
    relay = new TextInt<epicsInt32>("relay", this, /*base=*/10);
    post = new ConstStr("post", this, "?;FF", 4);
	*relay = relay_in;
	}
MsgSetSetpoint::MsgSetSetpoint(int relay_in)
    : Message("MsgSetSetpoint")
	{
    pre = new ConstStr("pre", this, "@253SP", 6);
    relay = new TextInt<epicsInt32>("relay", this, /*base=*/10);
    pling = new ConstStr("pling", this, "!", 1);
    val = new TextFloat<epicsFloat64>("val", this, 1, true);
    post = new ConstStr("post", this, ";FF", 3);
	*relay = relay_in;
	}
MsgGetHysteresis::MsgGetHysteresis(int relay_in)
    : Message("MsgGetHysteresis")
	{
    pre = new ConstStr("pre", this, "@253SH", 6);
	relay = new TextInt<epicsInt32>("relay", this, /*base=*/10);
    post = new ConstStr("post", this, "?;FF", 4);
    *relay = relay_in;
	}
MsgSetHysteresis::MsgSetHysteresis(int relay_in)
    : Message("MsgSetHysteresis")
	{
    pre = new ConstStr("pre", this, "@253SH", 6);
	relay = new TextInt<epicsInt32>("relay", this, /*base=*/10);
    pling = new ConstStr("pling", this, "!", 1);
    val = new TextFloat<epicsFloat64>("val", this, 1, true);
    post = new ConstStr("post", this, ";FF", 3);
    *relay = relay_in;
	}
MsgGetSpDirection::MsgGetSpDirection(int relay_in)
    : Message("MsgGetSpDirection")
	{
    pre = new ConstStr("pre", this, "@253SD", 6);
	relay = new TextInt<epicsInt32>("relay", this, /*base=*/10);
    post = new ConstStr("post", this, "?;FF", 4);
    *relay = relay_in;
	}
MsgSetSpDirection::MsgSetSpDirection(int relay_in)
    : Message("MsgSetSpDirection")
	{
    pre = new ConstStr("pre", this, "@253SD", 6);
	relay = new TextInt<epicsInt32>("relay", this, /*base=*/10);
    pling = new ConstStr("pling", this, "!", 1);
    val = new TerminatedEnum("val", this, ";FF", "BELOW\0ABOVE\0\0");
    *relay = relay_in;
	}
MsgReplySpDirection::MsgReplySpDirection()
    : Message("MsgReplySpDirection")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "BELOW\0ABOVE\0\0");
	}
MsgGetSpEnable::MsgGetSpEnable(int relay_in)
    : Message("MsgGetSpEnable")
	{
    pre = new ConstStr("pre", this, "@253EN", 6);
	relay = new TextInt<epicsInt32>("relay", this, /*base=*/10);
    post = new ConstStr("post", this, "?;FF", 4);
    *relay = relay_in;
	}
MsgSetSpEnable::MsgSetSpEnable(int relay_in)
    : Message("MsgSetSpEnable")
	{
    pre = new ConstStr("pre", this, "@253EN", 6);
	relay = new TextInt<epicsInt32>("relay", this, /*base=*/10);
    pling = new ConstStr("pling", this, "!", 1);
    val = new TerminatedEnum("val", this, ";FF", "OFF\0ON\0\0");
    *relay = relay_in;
	}
MsgReplySpEnable::MsgReplySpEnable()
    : Message("MsgReplySpEnable")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "OFF\0ON\0\0");
	}
MsgGetSpStatus::MsgGetSpStatus(int relay_in)
    : Message("MsgGetSpStatus")
	{
	//    pre = new ConstStr("pre", this, "@253SS1?;FF", 11);
	pre = new ConstStr("pre", this, "@253SS", 6);
	relay = new TextInt<epicsInt32>("relay", this, /*base=*/10);
	post = new ConstStr("post", this, "?;FF", 4);
	*relay = relay_in;
	}
MsgReplySpStatus::MsgReplySpStatus()
    : Message("MsgReplySpStatus")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "CLEAR\0SET\0\0");
	}

MsgGetTransducerType::MsgGetTransducerType()
    : Message("MsgGetTransducerType")
	{
    pre = new ConstStr("pre", this, "@253DT?;FF", 10);
	}
MsgReplyTransducerType::MsgReplyTransducerType()
    : Message("MsgReplyTransducerType")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "LOADLOCK\0DUALTRANS\0MICROPIRANI\0UNUMAG\0DUALMAG\0QUADMAG\0\0");
	}
MsgGetTransducerStatus::MsgGetTransducerStatus()
    : Message("MsgGetTransducerStatus")
	{
    pre = new ConstStr("pre", this, "@253T?;FF", 9);
	}
MsgReplyTransducerStatus::MsgReplyTransducerStatus()
    : Message("MsgReplyTransducerStatus")
	{
	// The T command returns the MicroPirani sensor status as O for OK, M for MicroPirani fail or Z for Piezo fail
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "O\0M\0Z\0C\0G\0\0");
	}

MsgSetCCControl::MsgSetCCControl()
    : Message("MsgSetCCControl")
	{
    pre = new ConstStr("pre", this, "@253FP!", 7);
    val = new TerminatedEnum("val", this, ";FF", "OFF\0ON\0\0");
	}
MsgReplyCCControl::MsgReplyCCControl()
    : Message("MsgReplyCCControl")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "OFF\0ON\0\0");
	}

MsgSetGasType::MsgSetGasType()
    : Message("MsgSetGasType")
	{
    pre = new ConstStr("pre", this, "@253GT!", 7);
    val = new TerminatedEnum("val", this, ";FF", "NITROGEN\0AIR\0ARGON\0HELIUM\0HYDROGEN\0H2O\0NEON\0CO2\0XENON\0\0");
	}
MsgGetGasType::MsgGetGasType()
    : Message("MsgGetGasType")
	{
    pre = new ConstStr("pre", this, "@253GT?;FF", 10);
	}
MsgReplyGasType::MsgReplyGasType()
    : Message("MsgReplyGasType")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "NITROGEN\0AIR\0ARGON\0HELIUM\0HYDROGEN\0H2O\0NEON\0CO2\0XENON\0\0");
	}

MsgSetLock::MsgSetLock()
    : Message("MsgSetLock")
	{
    pre = new ConstStr("pre", this, "@253FD!", 7);
    val = new TerminatedEnum("val", this, ";FF", "UNLOCK\0LOCK\0\0");
	}
MsgGetLock::MsgGetLock()
    : Message("MsgGetLock")
	{
    pre = new ConstStr("pre", this, "@253FD?;FF", 10);
	}
MsgReplyLock::MsgReplyLock()
    : Message("MsgReplyLock")
	{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "UNLOCK\0LOCK\0\0");
	}


MsgGetCCPressureDose::MsgGetCCPressureDose()
    : Message("MsgGetCCPressureDose")
	{
    pre = new ConstStr("pre", this, "@253TIM3?;FF", 12);
	}


/** Constructor for readThread.
  * \param[in] owner The owner object
  */
mks9xx::ReadThread::ReadThread(mks9xx* owner)
    : thread(*this, "readThread", epicsThreadGetStackSize(epicsThreadStackMedium))
	{
    this->owner = owner;
    thread.start();
	}

/** Constructor.
* \param[in] portDriver The asyn port driver this proxy belongs to.
* \param[in] devicePortName Name of the asyn port connected to the device.
* \param[in] devicePortAddress Address of the device port (normally 0).
*/
mks9xx::mks9xx(const char* portName,
        const char* serialPortName, int serialPortAddress)
    : asynPortDriver(portName, 1 /*maxAddr*/, NUM_PARAMS,
        asynInt32Mask | asynFloat64Mask | asynDrvUserMask | asynOctetMask,
        asynInt32Mask | asynFloat64Mask | asynOctetMask,
        ASYN_CANBLOCK, /*ASYN_CANBLOCK=1, ASYN_MULTIDEVICE=0 */
        1, /*autoConnect*/ 0, /*default priority */ 0) /*default stack size*/
	{
    // Define the parameters

    createParam(nameConnection, asynParamInt32, &indexConnection);
    createParam(nameAnalogueOutputFormat1, asynParamInt32, &indexAnalogueOutputFormat1);
    createParam(nameAnalogueOutputFormat2, asynParamInt32, &indexAnalogueOutputFormat2);
    createParam(nameUnits, asynParamInt32, &indexUnits);
    createParam(nameUserTag, asynParamOctet, &indexUserTag);
    createParam(nameFirmwareVersion, asynParamOctet, &indexFirmwareVersion);
    createParam(nameHardwareVersion, asynParamOctet, &indexHardwareVersion);
    createParam(nameManufacturer, asynParamOctet, &indexManufacturer);
    createParam(nameModel, asynParamOctet, &indexModel);
    createParam(namePressure1, asynParamFloat64, &indexPressure1);
    createParam(namePressure2, asynParamFloat64, &indexPressure2);
    createParam(namePressure3, asynParamFloat64, &indexPressure3);
    createParam(namePressure4, asynParamFloat64, &indexPressure4);
    createParam(namePressure5, asynParamFloat64, &indexPressure5);
    createParam(nameSerialNumber, asynParamOctet, &indexSerialNumber);
    createParam(nameTimeOn, asynParamInt32, &indexTimeOn);
    createParam(nameTransducerTemperature, asynParamFloat64, &indexTransducerTemperature);
    createParam(nameSetPoint1, asynParamFloat64, &indexSetPoint1);
    createParam(nameSetPoint2, asynParamFloat64, &indexSetPoint2);
    createParam(nameSetPoint3, asynParamFloat64, &indexSetPoint3);
    createParam(nameSetPointHysteresis1, asynParamFloat64, &indexSetPointHysteresis1);
    createParam(nameSetPointHysteresis2, asynParamFloat64, &indexSetPointHysteresis2);
    createParam(nameSetPointHysteresis3, asynParamFloat64, &indexSetPointHysteresis3);
    createParam(nameSetPointDirection1, asynParamInt32, &indexSetPointDirection1);
    createParam(nameSetPointDirection2, asynParamInt32, &indexSetPointDirection2);
    createParam(nameSetPointDirection3, asynParamInt32, &indexSetPointDirection3);
    createParam(nameSetPointEnable1, asynParamInt32, &indexSetPointEnable1);
    createParam(nameSetPointEnable2, asynParamInt32, &indexSetPointEnable2);
    createParam(nameSetPointEnable3, asynParamInt32, &indexSetPointEnable3);
    createParam(nameSetPointState1, asynParamInt32, &indexSetPointState1);
    createParam(nameSetPointState2, asynParamInt32, &indexSetPointState2);
    createParam(nameSetPointState3, asynParamInt32, &indexSetPointState3);
    createParam(nameTransducerType, asynParamInt32, &indexTransducerType);
    createParam(nameTransducerStatus, asynParamInt32, &indexTransducerStatus);
    createParam(nameCCControl, asynParamInt32, &indexCCControl);
    createParam(nameGasType, asynParamInt32, &indexGasType);
    createParam(nameLock, asynParamInt32, &indexLock);
    createParam(nameCCPressureDose, asynParamFloat64, &indexCCPressureDose);
    createParam(nameSTA, asynParamInt32, &indexSTA);


    setIntegerParam(indexConnection, 0);
    setIntegerParam(indexAnalogueOutputFormat1, 0);
    setIntegerParam(indexAnalogueOutputFormat2, 0);
    setIntegerParam(indexUnits, 0);
    setStringParam(indexUserTag, "");
    setStringParam(indexFirmwareVersion, "");
    setStringParam(indexHardwareVersion, "");
    setStringParam(indexManufacturer, "");
    setStringParam(indexModel, "");
    setStringParam(indexSerialNumber, "");
    setDoubleParam(indexPressure1, 0.0);
    setDoubleParam(indexPressure2, 0.0);
    setDoubleParam(indexPressure3, 0.0);
    setDoubleParam(indexPressure4, 0.0);
    setDoubleParam(indexPressure5, 0.0);
    setDoubleParam(indexTransducerTemperature, 0.0);
    setDoubleParam(indexSetPoint1, 0.0);
    setDoubleParam(indexSetPoint2, 0.0);
    setDoubleParam(indexSetPoint3, 0.0);
    setDoubleParam(indexSetPointHysteresis1, 0.0);
    setDoubleParam(indexSetPointHysteresis2, 0.0);
    setDoubleParam(indexSetPointHysteresis3, 0.0);
    setDoubleParam(indexCCPressureDose, 0.0);
    setIntegerParam(indexTimeOn, 0);
    setIntegerParam(indexSetPointDirection1, 0);
    setIntegerParam(indexSetPointDirection2, 0);
    setIntegerParam(indexSetPointDirection3, 0);
    setIntegerParam(indexSetPointEnable1, 0);
    setIntegerParam(indexSetPointEnable2, 0);
    setIntegerParam(indexSetPointEnable3, 0);
    setIntegerParam(indexSetPointState1, 0);
    setIntegerParam(indexSetPointState2, 0);
    setIntegerParam(indexSetPointState3, 0);
    setIntegerParam(indexTransducerType, 0);
    setIntegerParam(indexTransducerStatus, 0);
    setIntegerParam(indexCCControl, 0);
    setIntegerParam(indexGasType, 0);
    setIntegerParam(indexLock, 0);
    setIntegerParam(indexSTA, 0);


    // Connect to the serial port
    asynStatus asynRtn = pasynOctetSyncIO->connect(serialPortName,
        serialPortAddress, &this->serialPortUser, NULL);
    if(asynRtn != asynSuccess)
    	{
        printf("Failed to connect to serial port=%s error=%d\n",
            serialPortName, asynRtn);
    	}
    pasynOctetSyncIO->flush(this->serialPortUser);
    pasynOctetSyncIO->setInputEos(this->serialPortUser,"FF",2);

    protocolAnOutputFormat = new Protocol("protocolAnOutputFormat", this->serialPortUser);
    //protocolAnOutputFormat->debug(5);
    protocolAnOutputFormat->addMessage(new MsgReplyAnOutputFormat);
    protocolAnOutputFormat->addMessage(new MsgFailReply);
    protocolUnit = new Protocol("protocolUnit", this->serialPortUser);
    protocolUnit->addMessage(new MsgReplyUnit);
    protocolUnit->addMessage(new MsgFailReply);
    protocolUserTag = new Protocol("protocolUserTag", this->serialPortUser);
    protocolUserTag->addMessage(new MsgStringReply);
    protocolUserTag->addMessage(new MsgFailReply);
    protocolFirmwareVersion = new Protocol("protocolFirmwareVersion", this->serialPortUser);
    protocolFirmwareVersion->addMessage(new MsgStringReply);
    protocolFirmwareVersion->addMessage(new MsgFailReply);
    protocolHardwareVersion = new Protocol("protocolHardwareVersion", this->serialPortUser);
    protocolHardwareVersion->addMessage(new MsgStringReply);
    protocolHardwareVersion->addMessage(new MsgFailReply);
    protocolManufacturer = new Protocol("protocolManufacturer", this->serialPortUser);
    protocolManufacturer->addMessage(new MsgStringReply);
    protocolManufacturer->addMessage(new MsgFailReply);
    protocolModel = new Protocol("protocolModel", this->serialPortUser);
    protocolModel->addMessage(new MsgStringReply);
    protocolModel->addMessage(new MsgFailReply);
    protocolPressure = new Protocol("protocolPressure", this->serialPortUser);
    protocolPressure->addMessage(new MsgFloatReply);
    protocolPressure->addMessage(new MsgFailReply);
    protocolSerialNumber = new Protocol("protocolSerialNumber", this->serialPortUser);
    protocolSerialNumber->addMessage(new MsgStringReply);
    protocolSerialNumber->addMessage(new MsgFailReply);
    protocolTimeOn = new Protocol("protocolTimeOn", this->serialPortUser);
    //protocolTimeOn->debug(5);
    protocolTimeOn->addMessage(new MsgIntReply);
    protocolTimeOn->addMessage(new MsgFailReply);
    protocolTransducerTemperature = new Protocol("protocolTransducerTemperature", this->serialPortUser);
    protocolTransducerTemperature->addMessage(new MsgFloatReply);
    protocolTransducerTemperature->addMessage(new MsgFailReply);
    protocolSetPoint = new Protocol("protocolSetPoint", this->serialPortUser);
    //protocolSetPoint->debug(5);
    protocolSetPoint->addMessage(new MsgFloatReply);
    protocolSetPoint->addMessage(new MsgFailReply);
    protocolHysteresis = new Protocol("protocolHysteresis", this->serialPortUser);
    protocolHysteresis->addMessage(new MsgFloatReply);
    protocolHysteresis->addMessage(new MsgFailReply);
    protocolSpDirection = new Protocol("protocolSpDirection", this->serialPortUser);
    //protocolSpDirection->debug(5);
    protocolSpDirection->addMessage(new MsgReplySpDirection);
    protocolSpDirection->addMessage(new MsgFailReply);
    protocolSpEnable = new Protocol("protocolSpEnable", this->serialPortUser);
    //protocolSpEnable->debug(5);
    protocolSpEnable->addMessage(new MsgReplySpEnable);
    protocolSpEnable->addMessage(new MsgFailReply);
    protocolSpStatus = new Protocol("protocolSpStatus", this->serialPortUser);
    protocolSpStatus->addMessage(new MsgReplySpStatus);
    protocolSpStatus->addMessage(new MsgFailReply);
    protocolTransducerType = new Protocol("protocolTransducerType", this->serialPortUser);
    //protocolTransducerType->debug(5);
    protocolTransducerType->addMessage(new MsgReplyTransducerType);
    protocolTransducerType->addMessage(new MsgFailReply);
    protocolTransducerStatus = new Protocol("protocolTransducerStatus", this->serialPortUser);
    protocolTransducerStatus->addMessage(new MsgReplyTransducerStatus);
    protocolTransducerStatus->addMessage(new MsgFailReply);
    protocolCCControl = new Protocol("protocolCCControl", this->serialPortUser);
    protocolCCControl->addMessage(new MsgReplyCCControl);
    protocolCCControl->addMessage(new MsgFailReply);
    protocolGasType = new Protocol("protocolGasType", this->serialPortUser);
    //protocolGasType->debug(5);
    protocolGasType->addMessage(new MsgReplyGasType);
    protocolGasType->addMessage(new MsgFailReply);
    protocolLock = new Protocol("protocolLock", this->serialPortUser);
    protocolLock->addMessage(new MsgReplyLock);
    protocolLock->addMessage(new MsgFailReply);
    protocolCCPressureDose = new Protocol("protocolCCPressureDose", this->serialPortUser);
    //protocolCCPressureDose->debug(5);
    protocolCCPressureDose->addMessage(new MsgFloatReply);
    protocolCCPressureDose->addMessage(new MsgFailReply);


    //Protocol::debug("protocolPressure", 5);

    // Instantiate the three relay setpoint messages
    for (int i = 0; i < NUM_RELAYS; i++)
    	{
        msgSetSetpoint[i] = new MsgSetSetpoint(i+1);
        msgGetSetpoint[i] = new MsgGetSetpoint(i+1);
        msgGetSpStatus[i] = new MsgGetSpStatus(i+1);
        msgGetHysteresis[i]  = new MsgGetHysteresis(i+1);
        msgSetHysteresis[i]  = new MsgSetHysteresis(i+1);
        msgSetSpDirection[i] = new MsgSetSpDirection(i+1);
        msgSetSpEnable[i]    = new MsgSetSpEnable(i+1);
        msgGetSpDirection[i] = new MsgGetSpDirection(i+1);
        msgGetSpEnable[i]    = new MsgGetSpEnable(i+1);
    	}
    
    // Instantiate the five sensor pressure reading messages
    for (int i = 0; i < NUM_SENSORS; i++)
    	{
        msgPressure[i] = new MsgGetPressure(i+1);
    	}

    for (int i = 0; i < NUM_AOCHANS; i++)
    	{
    	msgGetAnOutputFormat[i] = new MsgGetAnOutputFormat(i+1);
    	msgSetAnOutputFormat[i] = new MsgSetAnOutputFormat(i+1);
    	}

    this->transducerType = TT_UNDEFINED; // Until we beat the information out of the device!

    // Start the thread
    readIt = new ReadThread(this);
	}

/** Destructor.
*/
mks9xx::~mks9xx()
	{
    delete readIt;

    delete protocolAnOutputFormat;
    delete protocolUnit;
    delete protocolUserTag;
    delete protocolFirmwareVersion;
    delete protocolHardwareVersion;
    delete protocolManufacturer;
    delete protocolModel;
    delete protocolPressure;
    delete protocolSerialNumber;
    delete protocolTimeOn;
    delete protocolTransducerTemperature;
    delete protocolSetPoint;
    delete protocolHysteresis;
    delete protocolSpDirection;
    delete protocolSpEnable;
    delete protocolSpStatus;
    delete protocolTransducerType;
    delete protocolTransducerStatus;
    delete protocolCCControl;
    delete protocolGasType;

    for (int i = 0; i < NUM_RELAYS; i++)
    	{
    	delete msgSetSetpoint[i];
    	delete msgGetSetpoint[i];
    	delete msgGetSpStatus[i];
    	delete msgSetHysteresis[i];
    	delete msgSetSpDirection[i];
    	delete msgSetSpEnable[i];
    	delete msgGetHysteresis[i];
    	delete msgGetSpDirection[i];
    	delete msgGetSpEnable[i];
    	}
    for (int i = 0; i < NUM_SENSORS; i++)
    	{
    	delete msgPressure[i];
    	}

    for (int i = 0; i < NUM_AOCHANS; i++)
    	{
    	delete msgGetAnOutputFormat[i];
    	delete msgSetAnOutputFormat[i];
    	}
	}

/** This function will be run for the read thread.
  * Receive data from the device.
  */
void mks9xx::readRun()
	{
    // Forever...
    while(true)
    	{
        // Wait for next poll
        epicsThreadSleep(pollPeriod);
        // Get the connection state
        this->lock();
        int connected;
        this->getIntegerParam(indexConnection, &connected);
        this->unlock();
        // Poll the pressure
        bool ok = getFloat(protocolPressure, msgPressure[0], indexPressure1);
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "readRun\n");
        if(!ok && connected)
        	{
            printf("MKS9xx asyn driver: Device Disonnected");
            asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DEVICE, "Device disconnected\n");
            // Device became disconnected
            this->lock();
            this->setIntegerParam(indexConnection, 0);
            this->unlock();
        	}
        else if(ok && !connected)
        	{
            // Device became connected
            printf("MKS9xx asyn driver: Device Connected");
            asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DEVICE, "Device Connected\n");
            this->lock();
            this->setIntegerParam(indexConnection, 1);
            this->unlock();
            // Read all the once only stuff
            getTransducerType(protocolTransducerType, &msgTransducerType, indexTransducerType);
            getString(protocolFirmwareVersion, &msgFirmwareVersion, indexFirmwareVersion);
            getString(protocolHardwareVersion, &msgHardwareVersion, indexHardwareVersion);
            getString(protocolManufacturer, &msgManufacturer, indexManufacturer);
            getString(protocolModel, &msgModel, indexModel);
            getString(protocolSerialNumber, &msgSerialNumber, indexSerialNumber);
            getString(protocolUserTag, &msgUserTag, indexUserTag);
            getString(protocolLock, &msgGetLock, indexLock);
            getUnit(protocolUnit, &msgGetUnit, indexUnits);
            getGasType(protocolGasType, &msgGetGasType, indexGasType);

            for (int i = 0; i < NUM_RELAYS; i++)
            	{
				getFloat(protocolSetPoint, msgGetSetpoint[i], indexSetPoint1+i);
				getFloat(protocolHysteresis, msgGetHysteresis[i], indexSetPointHysteresis1+i);
				getSpEnable(protocolSpEnable, msgGetSpEnable[i], indexSetPointEnable1+i);
				getSpDirection(protocolSpDirection, msgGetSpDirection[i], indexSetPointDirection1+i);
            	}

            for (int i = 0; i < NUM_AOCHANS; i++)
            	{
            	getAnOutputFormat(protocolAnOutputFormat, msgGetAnOutputFormat[i], indexAnalogueOutputFormat1+i);
            	}
        	}
        if(ok)
        	{
            // Poll the rest of the parameters
            getInt(protocolTimeOn, &msgTimeOn, indexTimeOn);
            getFloat(protocolTransducerTemperature, &msgTransducerTemperature, indexTransducerTemperature);
            getSpStatus(protocolSpStatus, msgGetSpStatus[0], indexSetPointState1);
            getSpStatus(protocolSpStatus, msgGetSpStatus[1], indexSetPointState2);
            getSpStatus(protocolSpStatus, msgGetSpStatus[2], indexSetPointState3);
            getTransducerStatus(protocolTransducerStatus, &msgTransducerStatus, indexTransducerStatus);

            getGaugeStatus();

            // Already have pressure1 from outer conditional, so get remaining 4
            getFloat(protocolPressure, msgPressure[1], indexPressure2);
            getFloat(protocolPressure, msgPressure[2], indexPressure3);
            getFloat(protocolPressure, msgPressure[3], indexPressure4);
            getFloat(protocolPressure, msgPressure[4], indexPressure5);

            // Cold Cathode pressure dose is only available for the following transducer types
            if ((this->transducerType == TT_UNIMAG) || (this->transducerType == TT_DUALMAG) || (this->transducerType == TT_QUADMAG))
				{
				getFloat(protocolCCPressureDose, &msgGetCCPressureDose, indexCCPressureDose);
				}
        	}
        // Tell EPICS
        this->lock();
        this->callParamCallbacks();
        this->unlock();
    	}
	}

/** Sends a set message to the device containing an integer parameter.
 *  \param[in] protocol The set of return messages
 *  \param[in] cmd The message to send
 *  \param[in] valField The message field into which to place the value
 *  \param[in] handle The index of the asyn parameter
 *  \return True for success
 */
bool mks9xx::setInt(Protocol* protocol, Message* cmd, TerminatedEnum* valField, int handle)
{
    int val;
    lock();
    getIntegerParam(handle, &val);
    unlock();
    *valField = val;
    return protocol->transceive(cmd) != NULL;
}
bool mks9xx::setInt(Protocol* protocol, Message* cmd, TextInt<epicsInt32>* valField, int handle)
	{
    int val;
    lock();
    getIntegerParam(handle, &val);
    unlock();
    *valField = val;
    return protocol->transceive(cmd) != NULL;
	}

/** Sends a set message to the device containing an integer parameter.
 *  \param[in] protocol The set of return messages
 *  \param[in] cmd The message to send
 *  \param[in] valField The message field into which to place the value
 *  \param[in] handle The index of the asyn parameter
 *  \return True for success
 */
bool mks9xx::setFloat(Protocol* protocol, Message* cmd, TextFloat<epicsFloat64>* valField, int handle)
{
    double val;
    lock();
    getDoubleParam(handle, &val);
    unlock();
    *valField = val;
    return protocol->transceive(cmd) != NULL;
}

/** Sends a set message to the device containing a string parameter.
 *  \param[in] protocol The set of return messages
 *  \param[in] cmd The message to send
 *  \param[in] valField The message field into which to place the value
 *  \param[in] handle The index of the asyn parameter
 *  \return True for success
 */
bool mks9xx::setString(Protocol* protocol, Message* cmd, TerminatedStr* valField, int handle)
{
    char val[userTagLength+1];
    lock();
    getStringParam(handle, userTagLength, val);
    val[userTagLength] = '\0';
    unlock();
    *valField = val;
    return protocol->transceive(cmd) != NULL;
}

/** Sends a get message to the device and places the string result in the
 *  asyn parameter.
 *  \param[in] protocol The set of return messages
 *  \param[in] cmd The message to send
 *  \param[in] handle The index of the asyn parameter
 *  \return True for success
 */
bool mks9xx::getString(Protocol* protocol, Message* cmd, int handle)
{
    bool result = false;
    Message* msg = protocol->transceive(cmd);
    if(protocol->isMessageKind<MsgStringReply>(msg))
    {
        MsgStringReply* reply = dynamic_cast<MsgStringReply*>(msg);
        std::string text = (const char*)*reply->val;
        size_t lastSpacePos = text.find_last_not_of(' ');
        text = text.substr(0,lastSpacePos+1);
        lock();
        setStringParam(handle, text.c_str());
        unlock();
        result = true;
    }
    return result;
}

/** Sends a get message to the device and places the float result in the
 *  asyn parameter.
 *  \param[in] protocol The set of return messages
 *  \param[in] cmd The message to send
 *  \param[in] handle The index of the asyn parameter
 *  \return True for success
 */
bool mks9xx::getFloat(Protocol* protocol, Message* cmd, int handle)
{
    bool result = false;
    Message* msg = protocol->transceive(cmd);
    if(protocol->isMessageKind<MsgFloatReply>(msg))
    {
        MsgFloatReply* reply = dynamic_cast<MsgFloatReply*>(msg);
        lock();
        setDoubleParam(handle, *reply->val);
        unlock();
        result = true;
    }
    return result;
}

/** Sends a get message to the device and places the float result in the
 *  asyn parameter.
 *  \param[in] protocol The set of return messages
 *  \param[in] cmd The message to send
 *  \param[in] handle The index of the asyn parameter
 *  \return True for success
 */
bool mks9xx::getInt(Protocol* protocol, Message* cmd, int handle)
{
    bool result = false;
    Message* msg = protocol->transceive(cmd);
    if(protocol->isMessageKind<MsgIntReply>(msg))
    {
        MsgIntReply* reply = dynamic_cast<MsgIntReply*>(msg);
        lock();
        setIntegerParam(handle, *reply->val);
        unlock();
        result = true;
    }
    return result;
}

/** Sends a get message to the device and places the float result in the
 *  asyn parameter.
 *  \param[in] protocol The set of return messages
 *  \param[in] cmd The message to send
 *  \param[in] handle The index of the asyn parameter
 *  \return True for success
 */
bool mks9xx::getSpStatus(Protocol* protocol, Message* cmd, int handle)
{
    bool result = false;
    Message* msg = protocol->transceive(cmd);
    if(protocol->isMessageKind<MsgReplySpStatus>(msg))
    {
        MsgReplySpStatus* reply = dynamic_cast<MsgReplySpStatus*>(msg);
        lock();
        setIntegerParam(handle, *reply->val);
        unlock();
        result = true;
    }
    return result;
}

/** Sends a get message to the device and places the float result in the
 *  asyn parameter.
 *  \param[in] protocol The set of return messages
 *  \param[in] cmd The message to send
 *  \param[in] handle The index of the asyn parameter
 *  \return True for success
 */
bool mks9xx::getTransducerStatus(Protocol* protocol, Message* cmd, int handle)
{
    bool result = false;
    Message* msg = protocol->transceive(cmd);
    if(protocol->isMessageKind<MsgReplyTransducerStatus>(msg))
    {
        MsgReplyTransducerStatus* reply = dynamic_cast<MsgReplyTransducerStatus*>(msg);
        lock();
        setIntegerParam(handle, *reply->val);
        unlock();
        result = true;
    }
    return result;
}

/** Sends a get message to the device and places the float result in the
 *  asyn parameter.
 *  \param[in] protocol The set of return messages
 *  \param[in] cmd The message to send
 *  \param[in] handle The index of the asyn parameter
 *  \return True for success
 */
bool mks9xx::getTransducerType(Protocol* protocol, Message* cmd, int handle)
{
    bool result = false;
    Message* msg = protocol->transceive(cmd);
    if(protocol->isMessageKind<MsgReplyTransducerType>(msg))
    {
        MsgReplyTransducerType* reply = dynamic_cast<MsgReplyTransducerType*>(msg);
        lock();
        setIntegerParam(handle, *reply->val);
        unlock();
        // Transducer type will not change, so cache it locally for future reference.
        this->transducerType = *reply->val;
        result = true;
    }
    return result;
}

bool mks9xx::getUnit(Protocol* protocol, Message* cmd, int handle)
{
    bool result = false;
    Message* msg = protocol->transceive(cmd);
    if(protocol->isMessageKind<MsgReplyUnit>(msg))
    {
        MsgReplyUnit* reply = dynamic_cast<MsgReplyUnit*>(msg);
        lock();
        setIntegerParam(handle, *reply->val);
        unlock();
        result = true;
    }
    return result;
}

bool mks9xx::getGasType(Protocol* protocol, Message* cmd, int handle)
{
    bool result = false;
    Message* msg = protocol->transceive(cmd);
    if(protocol->isMessageKind<MsgReplyGasType>(msg))
    {
        MsgReplyGasType* reply = dynamic_cast<MsgReplyGasType*>(msg);
        lock();
        setIntegerParam(handle, *reply->val);
        unlock();
        result = true;
    }
    return result;
}

bool mks9xx::getSpEnable(Protocol* protocol, Message* cmd, int handle)
{
    bool result = false;
    Message* msg = protocol->transceive(cmd);
    if(protocol->isMessageKind<MsgReplySpEnable>(msg))
    {
        MsgReplySpEnable* reply = dynamic_cast<MsgReplySpEnable*>(msg);
        lock();
        setIntegerParam(handle, *reply->val);
        unlock();
        result = true;
    }
    return result;
}

bool mks9xx::getSpDirection(Protocol* protocol, Message* cmd, int handle)
{
    bool result = false;
    Message* msg = protocol->transceive(cmd);
    if(protocol->isMessageKind<MsgReplySpDirection>(msg))
    {
        MsgReplySpDirection* reply = dynamic_cast<MsgReplySpDirection*>(msg);
        lock();
        setIntegerParam(handle, *reply->val);
        unlock();
        result = true;
    }
    return result;
}

bool mks9xx::getAnOutputFormat(Protocol* protocol, Message* cmd, int handle)
{
    bool result = false;
    Message* msg = protocol->transceive(cmd);
    if(protocol->isMessageKind<MsgReplyAnOutputFormat>(msg))
    {
    	MsgReplyAnOutputFormat* reply = dynamic_cast<MsgReplyAnOutputFormat*>(msg);
        lock();
        setIntegerParam(handle, *reply->val);
        unlock();
        result = true;
    }
    return result;
}

/** Determines the overall gauge status based on multiple parameters
 *
 *  \return True for success
 */
bool mks9xx::getGaugeStatus()
	{
    bool result = false;
    int connected = 0;
    int transStatus = 0;
    int sta = STA_FAULT;
    this->getIntegerParam(indexConnection, &connected);
    this->getIntegerParam(indexTransducerStatus, &transStatus);

    if (connected)
    	{
    	if (transStatus == 0 || transStatus == 4)
    		{
    		sta = STA_OK;
    		}
    	else
			{
			sta = STA_TRANSFAULT;
			}
    	}
    else
    	sta = STA_NOTCONNECTED;

    lock();
	setIntegerParam(indexSTA, sta);
	unlock();

	result = true;

	return result;
	}

/** Called when asyn clients call pasynInt32->write().
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Value to write. */
asynStatus mks9xx::writeInt32(asynUser *pasynUser, epicsInt32 value)
	{
    // Base class does most of the work including setting the parameter library
    asynStatus result = asynPortDriver::writeInt32(pasynUser, value);

    lock();
    // Send commands that control configuration.
    int parameter = pasynUser->reason;
    if ((parameter >= indexAnalogueOutputFormat1) && (parameter <= indexAnalogueOutputFormat2))
    	{
    	int iChannel = parameter - indexAnalogueOutputFormat1;
        setInt(protocolAnOutputFormat, msgSetAnOutputFormat[iChannel], msgSetAnOutputFormat[iChannel]->val, parameter);
    	}
    else if((parameter >= indexSetPointDirection1) && (parameter <= indexSetPointDirection3))
    	{
    	int iRelay = parameter - indexSetPointDirection1;
        setInt(protocolSpDirection, msgSetSpDirection[iRelay], msgSetSpDirection[iRelay]->val, parameter);
    	}
    else if((parameter >= indexSetPointEnable1) && (parameter <= indexSetPointEnable3))
    	{
    	int iRelay = parameter - indexSetPointEnable1;
        setInt(protocolSpEnable, msgSetSpEnable[iRelay], msgSetSpEnable[iRelay]->val, parameter);
    	}
    else if(parameter == indexUnits)
    	{
        setInt(protocolUnit, &msgSetUnit, msgSetUnit.val, indexUnits);
    	}
    else if(parameter == indexCCControl)
    	{
        setInt(protocolCCControl, &msgCCControl, msgCCControl.val, indexCCControl);
    	}
    else if(parameter == indexGasType)
    	{
        printf("writeInt32: Gas Type");
        setInt(protocolGasType, &msgSetGasType, msgSetGasType.val, indexGasType);
    	}
    else if(parameter == indexLock)
    	{
        setInt(protocolLock, &msgSetLock, msgSetLock.val, indexLock);
    	}
    unlock();
    return result;
	}

/** Called when asyn clients call pasynFloat64->write().
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Value to write. */
asynStatus mks9xx::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
	{
    // Base class does most of the work including setting the parameter library
    asynStatus result = asynPortDriver::writeFloat64(pasynUser, value);

    lock();
    // Send commands that control configuration.
    int parameter = pasynUser->reason;
    if((parameter >= indexSetPointHysteresis1) && (parameter <= indexSetPointHysteresis3))
    	{
    	int iRelay = parameter - indexSetPointHysteresis1;
        setFloat(protocolHysteresis, msgSetHysteresis[iRelay], msgSetHysteresis[iRelay]->val, parameter);
    	}
    else if(parameter == indexSetPoint1)
    	{
        setFloat(protocolSetPoint, msgSetSetpoint[0], msgSetSetpoint[0]->val, indexSetPoint1);
        //asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "writeFloat64(): SETPOINT1 - returned from setFloat\n");
    	}
    else if(parameter == indexSetPoint2)
    	{
        setFloat(protocolSetPoint, msgSetSetpoint[1], msgSetSetpoint[1]->val, indexSetPoint2);
    	}
    else if(parameter == indexSetPoint3)
    	{
        setFloat(protocolSetPoint, msgSetSetpoint[2], msgSetSetpoint[2]->val, indexSetPoint3);
    	}
    unlock();
    return result;
	}

/** Called when asyn clients call pasynOctet->write().
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Value to write.
  * \param[in] maxChars Max characters to be written
  * \param[out] nActual Number of characters actually written */
asynStatus mks9xx::writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual)
	{
    // Base class does most of the work including setting the parameter library
    asynStatus result = asynPortDriver::writeOctet(pasynUser, value, maxChars, nActual);

    lock();
    // Send commands that control configuration.
    int parameter = pasynUser->reason;
    if(parameter == indexUserTag)
    	{
        setString(protocolUserTag, &msgSetUserTag, msgSetUserTag.val, indexUserTag);
        getString(protocolUserTag, &msgUserTag, indexUserTag);
    	}
    unlock();
    return result;
	}

/** Configuration command, called directly or from iocsh.
  * \param[in] portName The name of this asyn port.
  * \param[in] serialPortName The name of the serial port connected to the device.
  * \param[in] serialPortAddress The address of the serial port (usually 0).
  */
extern "C" int mks9xxConfig(const char *portName,
    const char* serialPortName, int serialPortAddress)
{
    new mks9xx(portName, serialPortName, serialPortAddress);
    return(asynSuccess);
}

/* Code for iocsh registration */
static const iocshArg mks9xxConfigArg0 = {"Port name", iocshArgString};
static const iocshArg mks9xxConfigArg1 = {"Serial port name", iocshArgString};
static const iocshArg mks9xxConfigArg2 = {"Serial port address", iocshArgInt};
static const iocshArg* const mks9xxConfigArgs[] =
    {&mks9xxConfigArg0, &mks9xxConfigArg1, &mks9xxConfigArg2};
static const iocshFuncDef configmks9xx =
    {"mks9xxConfig", 3, mks9xxConfigArgs};
static void configmks9xxCallFunc(const iocshArgBuf *args)
{
    mks9xxConfig(args[0].sval, args[1].sval, args[2].ival);
}

static void mks9xxRegister(void)
{
    iocshRegister(&configmks9xx, configmks9xxCallFunc);
}

extern "C" { epicsExportRegistrar(mks9xxRegister); }

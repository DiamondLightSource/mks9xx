
/*
* mks9xx.cpp
*
*/

#include "mks9xx.h"
#include <asynOctetSyncIO.h>
#include <iocsh.h>
#include <epicsExport.h>

// Parameter name definitions
//*!*Section stringDefinition begin*!*
const char* mks9xx::nameConnection = "CONNECTION";
const char* mks9xx::nameAnalogueOutputFormat = "ANALOGUEOUTPUTFORMAT";
const char* mks9xx::nameUnits = "UNITS";
const char* mks9xx::nameUserTag = "USERTAG";
const char* mks9xx::nameTransducerType = "TRANSDUCERTYPE";
const char* mks9xx::nameFirmwareVersion = "FIRMWAREVERSION";
const char* mks9xx::nameHardwareVersion = "HARDWAREVERSION";
const char* mks9xx::nameManufacturer = "MANUFACTURER";
const char* mks9xx::nameModel = "MODEL";
const char* mks9xx::namePressure = "PRESSURE";
const char* mks9xx::nameSerialNumber = "SERIALNUMBER";
const char* mks9xx::nameTimeOn = "TIMEON";
const char* mks9xx::nameTransducerTemperature = "TRANSDUCERTEMPERATURE";
const char* mks9xx::nameSetPoint = "SETPOINT";
const char* mks9xx::nameSetPointHysteresis = "SETPOINTHYSTERESIS";
const char* mks9xx::nameSetPointDirection = "SETPOINTDIRECTION";
const char* mks9xx::nameSetPointEnable = "SETPOINTENABLE";
const char* mks9xx::nameSetPointState = "SETPOINTSTATE";
//*!*Section stringDefinition end*!*
#define NUM_PARAMS (&LAST_PARAM - &FIRST_PARAM - 1)
const double mks9xx::pollPeriod = 1.0;
const int mks9xx::userTagLength = 30;

//*!*Section messageDefinition begin*!*
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
    val = new TextFloat<epicsFloat64>("val", this, 1, false);
    post = new ConstStr("post", this, ";FF", 3);
}
MsgGetAnOutputFormat::MsgGetAnOutputFormat()
    : Message("MsgGetAnOutputFormat")
{
    pre = new ConstStr("pre", this, "@253DAC?;FF", 11);
}
MsgSetAnOutputFormat::MsgSetAnOutputFormat()
    : Message("MsgSetAnOutputFormat")
{
    pre = new ConstStr("pre", this, "@253DAC!", 8);
    val = new TerminatedEnum("val", this, ";FF", "Log5\0Log10\0Linear5\0Linear10\0\0");
}
MsgReplyAnOutputFormat::MsgReplyAnOutputFormat()
    : Message("MsgReplyAnOutputFormat")
{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "Log5\0Log10\0Linear5\0Linear10\0\0");
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
    val = new TerminatedEnum("val", this, ";FF", "Torr\0mBar\0Pascal\0\0");
}
MsgReplyUnit::MsgReplyUnit()
    : Message("MsgReplyUnit")
{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "Torr\0mBar\0Pascal\0\0");
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
MsgGetTransducerType::MsgGetTransducerType()
    : Message("MsgGetTransducerType")
{
    pre = new ConstStr("pre", this, "@253DT?;FF", 10);
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
MsgGetPressure::MsgGetPressure()
    : Message("MsgGetPressure")
{
    pre = new ConstStr("pre", this, "@253PR1?;FF", 11);
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
MsgGetSetPoint::MsgGetSetPoint()
    : Message("MsgGetSetPoint")
{
    pre = new ConstStr("pre", this, "@253SP1?;FF", 11);
}
MsgSetSetPoint::MsgSetSetPoint()
    : Message("MsgSetSetPoint")
{
    pre = new ConstStr("pre", this, "@253SP1!", 8);
    val = new TextFloat<epicsFloat64>("val", this, 1, false);
    post = new ConstStr("post", this, ";FF", 3);
}
MsgGetHysteresis::MsgGetHysteresis()
    : Message("MsgGetHysteresis")
{
    pre = new ConstStr("pre", this, "@253SH1?;FF", 11);
}
MsgSetHysteresis::MsgSetHysteresis()
    : Message("MsgSetHysteresis")
{
    pre = new ConstStr("pre", this, "@253SH1!", 8);
    val = new TextFloat<epicsFloat64>("val", this, 1, false);
    post = new ConstStr("post", this, ";FF", 3);
}
MsgGetSpDirection::MsgGetSpDirection()
    : Message("MsgGetSpDirection")
{
    pre = new ConstStr("pre", this, "@253SD1?;FF", 11);
}
MsgSetSpDirection::MsgSetSpDirection()
    : Message("MsgSetSpDirection")
{
    pre = new ConstStr("pre", this, "@253SD1!", 8);
    val = new TerminatedEnum("val", this, ";FF", "Below\0Above\0\0");
}
MsgReplySpDirection::MsgReplySpDirection()
    : Message("MsgReplySpDirection")
{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "Below\0Above\0\0");
}
MsgGetSpEnable::MsgGetSpEnable()
    : Message("MsgGetSpEnable")
{
    pre = new ConstStr("pre", this, "@253EN1?;FF", 11);
}
MsgSetSpEnable::MsgSetSpEnable()
    : Message("MsgSetSpEnable")
{
    pre = new ConstStr("pre", this, "@253EN1!", 8);
    val = new TerminatedEnum("val", this, ";FF", "Off\0On\0\0");
}
MsgReplySpEnable::MsgReplySpEnable()
    : Message("MsgReplySpEnable")
{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "Off\0On\0\0");
}
MsgGetSpStatus::MsgGetSpStatus()
    : Message("MsgGetSpStatus")
{
    pre = new ConstStr("pre", this, "@253SS1?;FF", 11);
}
MsgReplySpStatus::MsgReplySpStatus()
    : Message("MsgReplySpStatus")
{
    pre = new ConstStr("pre", this, "@253ACK", 7);
    val = new TerminatedEnum("val", this, ";FF", "CLEAR\0SET\0\0");
}
//*!*Section messageDefinition end*!*

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
    //*!*Section createParameter begin*!*
    createParam(nameConnection, asynParamInt32, &indexConnection);
    createParam(nameAnalogueOutputFormat, asynParamInt32, &indexAnalogueOutputFormat);
    createParam(nameUnits, asynParamInt32, &indexUnits);
    createParam(nameUserTag, asynParamOctet, &indexUserTag);
    createParam(nameTransducerType, asynParamOctet, &indexTransducerType);
    createParam(nameFirmwareVersion, asynParamOctet, &indexFirmwareVersion);
    createParam(nameHardwareVersion, asynParamOctet, &indexHardwareVersion);
    createParam(nameManufacturer, asynParamOctet, &indexManufacturer);
    createParam(nameModel, asynParamOctet, &indexModel);
    createParam(namePressure, asynParamFloat64, &indexPressure);
    createParam(nameSerialNumber, asynParamOctet, &indexSerialNumber);
    createParam(nameTimeOn, asynParamFloat64, &indexTimeOn);
    createParam(nameTransducerTemperature, asynParamFloat64, &indexTransducerTemperature);
    createParam(nameSetPoint, asynParamFloat64, &indexSetPoint);
    createParam(nameSetPointHysteresis, asynParamFloat64, &indexSetPointHysteresis);
    createParam(nameSetPointDirection, asynParamInt32, &indexSetPointDirection);
    createParam(nameSetPointEnable, asynParamInt32, &indexSetPointEnable);
    createParam(nameSetPointState, asynParamInt32, &indexSetPointState);
    //*!*Section createParameter end*!*
    //*!*Section initParameter begin*!*
    setIntegerParam(indexConnection, 0);
    setIntegerParam(indexAnalogueOutputFormat, 0);
    setIntegerParam(indexUnits, 0);
    setStringParam(indexUserTag, "");
    setStringParam(indexTransducerType, "");
    setStringParam(indexFirmwareVersion, "");
    setStringParam(indexHardwareVersion, "");
    setStringParam(indexManufacturer, "");
    setStringParam(indexModel, "");
    setDoubleParam(indexPressure, 0.0);
    setStringParam(indexSerialNumber, "");
    setDoubleParam(indexTimeOn, 0.0);
    setDoubleParam(indexTransducerTemperature, 0.0);
    setDoubleParam(indexSetPoint, 0.0);
    setDoubleParam(indexSetPointHysteresis, 0.0);
    setIntegerParam(indexSetPointDirection, 0);
    setIntegerParam(indexSetPointEnable, 0);
    setIntegerParam(indexSetPointState, 0);
    //*!*Section initParameter end*!*

    // Connect to the serial port
    asynStatus asynRtn = pasynOctetSyncIO->connect(serialPortName,
        serialPortAddress, &this->serialPortUser, NULL);
    if(asynRtn != asynSuccess)
    {
        printf("Failed to connect to serial port=%s error=%d\n",
            serialPortName, asynRtn);
    }
    pasynOctetSyncIO->flush(this->serialPortUser);

    //*!*Section protocolDefinition begin*!*
    protocolAnOutputFormat = new Protocol("protocolAnOutputFormat", this->serialPortUser);
    protocolAnOutputFormat->addMessage(new MsgReplyAnOutputFormat);
    protocolAnOutputFormat->addMessage(new MsgFailReply);
    protocolUnit = new Protocol("protocolUnit", this->serialPortUser);
    protocolUnit->addMessage(new MsgReplyUnit);
    protocolUnit->addMessage(new MsgFailReply);
    protocolUserTag = new Protocol("protocolUserTag", this->serialPortUser);
    protocolUserTag->addMessage(new MsgStringReply);
    protocolUserTag->addMessage(new MsgFailReply);
    protocolTransducerType = new Protocol("protocolTransducerType", this->serialPortUser);
    protocolTransducerType->addMessage(new MsgStringReply);
    protocolTransducerType->addMessage(new MsgFailReply);
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
    protocolTimeOn->addMessage(new MsgFloatReply);
    protocolTimeOn->addMessage(new MsgFailReply);
    protocolTransducerTemperature = new Protocol("protocolTransducerTemperature", this->serialPortUser);
    protocolTransducerTemperature->addMessage(new MsgFloatReply);
    protocolTransducerTemperature->addMessage(new MsgFailReply);
    protocolSetPoint = new Protocol("protocolSetPoint", this->serialPortUser);
    protocolSetPoint->addMessage(new MsgFloatReply);
    protocolSetPoint->addMessage(new MsgFailReply);
    protocolHysteresis = new Protocol("protocolHysteresis", this->serialPortUser);
    protocolHysteresis->addMessage(new MsgFloatReply);
    protocolHysteresis->addMessage(new MsgFailReply);
    protocolSpDirection = new Protocol("protocolSpDirection", this->serialPortUser);
    protocolSpDirection->addMessage(new MsgReplySpDirection);
    protocolSpDirection->addMessage(new MsgFailReply);
    protocolSpEnable = new Protocol("protocolSpEnable", this->serialPortUser);
    protocolSpEnable->addMessage(new MsgReplySpEnable);
    protocolSpEnable->addMessage(new MsgFailReply);
    protocolSpStatus = new Protocol("protocolSpStatus", this->serialPortUser);
    protocolSpStatus->addMessage(new MsgReplySpStatus);
    protocolSpStatus->addMessage(new MsgFailReply);
    //*!*Section protocolDefinition end*!*
    
    // Start the thread
    readIt = new ReadThread(this);
}

/** Destructor.
*/
mks9xx::~mks9xx()
{
    delete readIt;
    //*!*Section protocolDestruction begin*!*
    delete protocolAnOutputFormat;
    delete protocolUnit;
    delete protocolUserTag;
    delete protocolTransducerType;
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
    //*!*Section protocolDestruction end*!*
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
        bool ok = getFloat(protocolPressure, &msgPressure, indexPressure);
        if(!ok && connected)
        {
            // Device became disconnected
            this->lock();
            this->setIntegerParam(indexConnection, 0);
            this->unlock();
        }
        else if(ok && !connected)
        {
            // Device became connected
            this->lock();
            this->setIntegerParam(indexConnection, 1);
            this->unlock();
            // Read all the once only stuff
            getString(protocolTransducerType, &msgTransducerType, indexTransducerType);
            getString(protocolFirmwareVersion, &msgFirmwareVersion, indexFirmwareVersion);
            getString(protocolHardwareVersion, &msgHardwareVersion, indexHardwareVersion);
            getString(protocolManufacturer, &msgManufacturer, indexManufacturer);
            getString(protocolModel, &msgModel, indexModel);
            getString(protocolSerialNumber, &msgSerialNumber, indexSerialNumber);
            getString(protocolUserTag, &msgUserTag, indexUserTag);
            // Send the current configuration
            setInt(protocolAnOutputFormat, &msgAnOutputFormat, msgAnOutputFormat.val, indexAnalogueOutputFormat);
            setFloat(protocolHysteresis, &msgHysteresis, msgHysteresis.val, indexSetPointHysteresis);
            setFloat(protocolSetPoint, &msgSetPoint, msgSetPoint.val, indexSetPoint);
            setInt(protocolSpDirection, &msgSpDirection, msgSpDirection.val, indexSetPointDirection);
            setInt(protocolSpEnable, &msgSpEnable, msgSpEnable.val, indexSetPointEnable);
            setInt(protocolUnit, &msgUnit, msgUnit.val, indexUnits);
        }
        if(ok)
        {
            // Poll the rest of the parameters
            getFloat(protocolTimeOn, &msgTimeOn, indexTimeOn);
            getFloat(protocolTransducerTemperature, &msgTransducerTemperature, indexTransducerTemperature);
            getSpStatus(protocolSpStatus, &msgSpStatus, indexSetPointState);
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

/** Called when asyn clients call pasynInt32->write().
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Value to write. */
asynStatus mks9xx::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    // Base class does most of the work including setting the parameter library
    asynStatus result = asynPortDriver::writeInt32(pasynUser, value);

    // Send commands that control configuration.
    int parameter = pasynUser->reason;
    if(parameter == indexAnalogueOutputFormat)
    {
        setInt(protocolAnOutputFormat, &msgAnOutputFormat, msgAnOutputFormat.val, indexAnalogueOutputFormat);
    }
    else if(parameter == indexSetPointDirection)
    {
        setInt(protocolSpDirection, &msgSpDirection, msgSpDirection.val, indexSetPointDirection);
    }
    else if(parameter == indexSetPointEnable)
    {
        setInt(protocolSpEnable, &msgSpEnable, msgSpEnable.val, indexSetPointEnable);
    }
    else if(parameter == indexUnits)
    {
        setInt(protocolUnit, &msgUnit, msgUnit.val, indexUnits);
    }
    return result;
}

/** Called when asyn clients call pasynFloat64->write().
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Value to write. */
asynStatus mks9xx::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    // Base class does most of the work including setting the parameter library
    asynStatus result = asynPortDriver::writeFloat64(pasynUser, value);

    // Send commands that control configuration.
    int parameter = pasynUser->reason;
    if(parameter == indexSetPointHysteresis)
    {
        setFloat(protocolHysteresis, &msgHysteresis, msgHysteresis.val, indexSetPointHysteresis);
    }
    else if(parameter == indexSetPoint)
    {
        setFloat(protocolSetPoint, &msgSetPoint, msgSetPoint.val, indexSetPoint);
    }
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

    // Send commands that control configuration.
    int parameter = pasynUser->reason;
    if(parameter == indexUserTag)
    {
        setString(protocolUserTag, &msgSetUserTag, msgSetUserTag.val, indexUserTag);
        getString(protocolUserTag, &msgUserTag, indexUserTag);
    }
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

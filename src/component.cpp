 #include "component.h"

std::list<Component*> Component::instances = {};

Component::Component(const char* name, uint16_t goOffset)
    : name(name), goOffset(goOffset)
{
   instances.push_back(this);
}

Component::~Component()
{
    instances.remove(this);
} 

void Component::loopAll(unsigned long now, bool initalize)
{
    for (std::list<Component*>::iterator it=instances.begin(); it != instances.end(); ++it)
    {
        (*it)->loop(now, initalize);
    }
}

void Component::receiveAll(GroupObject& groupObject)
{
    if (ArduinoPlatform::SerialDebug != NULL)
    {
        ArduinoPlatform::SerialDebug->print("Received Group Object ");
        ArduinoPlatform::SerialDebug->println(groupObject.asap());
    }
    for (std::list<Component*>::iterator it=instances.begin(); it != instances.end(); ++it)
    {
        (*it)->received(groupObject);
    }
}


uint8_t Component::readKnxParameterUInt8(const char* operation, uint32_t parameterAddress)
{
    if (ArduinoPlatform::SerialDebug != NULL)
    {
        ArduinoPlatform::SerialDebug->print("KNX Parameter Offset ");
        ArduinoPlatform::SerialDebug->print(parameterAddress);
        ArduinoPlatform::SerialDebug->print(": ");
    }
    uint8_t result = knx.paramByte(parameterAddress);
    logValue(NULL, operation, result);
    return result;
}

uint32_t Component::readKnxParameterUInt32(const char* operation, uint32_t parameterAddress)
{
    if (ArduinoPlatform::SerialDebug != NULL)
    {
        ArduinoPlatform::SerialDebug->print("KNX Parameter Offset ");
        ArduinoPlatform::SerialDebug->print(parameterAddress);
        ArduinoPlatform::SerialDebug->print(": ");
    }
    uint32_t result = knx.paramInt(parameterAddress);
    logValue(NULL, operation, result);
    return result;
}

float Component::readKnxParameterFloat(const char* operation, uint32_t parameterAddress)
{
    if (ArduinoPlatform::SerialDebug != NULL)
    {
        ArduinoPlatform::SerialDebug->print("KNX Parameter Offset ");
        ArduinoPlatform::SerialDebug->print(parameterAddress);
        ArduinoPlatform::SerialDebug->print(": ");
    }
    float result = knx.paramFloat(parameterAddress, ParameterFloatEncodings::Float_Enc_IEEE754Single);
    logValue(NULL, operation, result);
    return result;
}

void Component::readKnxParameterString(const char* operation, uint32_t parameterAddress, char* buffer, size_t chars)
{
    Component::readKnxParameterString(name, operation, parameterAddress, buffer, chars);
}

void Component::readKnxParameterString(const char* name, const char* operation, uint32_t parameterAddress, char* buffer, size_t bufferSize)
{
    size_t chars = bufferSize - 1;
    if (ArduinoPlatform::SerialDebug != NULL)
    {
        ArduinoPlatform::SerialDebug->print("KNX Parameter Offset ");
        ArduinoPlatform::SerialDebug->print(parameterAddress);
        ArduinoPlatform::SerialDebug->print(": ");
    }
    char* result = (char*) knx.paramData(parameterAddress);
    memcpy(buffer, result, chars);
    buffer[chars] = 0;
    if (ArduinoPlatform::SerialDebug != NULL)
    {
        ArduinoPlatform::SerialDebug->print("'");
        ArduinoPlatform::SerialDebug->print(buffer);
        ArduinoPlatform::SerialDebug->println("'");
    }
}

void Component::logValue(const char* goName, const char* operation, float value)
{
    if (ArduinoPlatform::SerialDebug != NULL)
    {
        ArduinoPlatform::SerialDebug->print(name);
        ArduinoPlatform::SerialDebug->print(" ");
        if (goName != NULL)
        {
            ArduinoPlatform::SerialDebug->print(goName);
            ArduinoPlatform::SerialDebug->print(" "); 
        }
        ArduinoPlatform::SerialDebug->print(operation);
        ArduinoPlatform::SerialDebug->print(" ");
        ArduinoPlatform::SerialDebug->println(value);
    }
}

bool Component::isGo(GroupObject& groupObject, uint16_t localGoNr, const char* goName, const Dpt& dpt)
{
    GroupObject& go = knx.getGroupObject(localGoNr + goOffset);
    if (groupObject.asap() == go.asap())
    {
        const KNXValue& value = groupObject.value(dpt);
        logValue(goName, "Received", value);
        return true;
    }
    return false;
}

bool Component::goSet(uint16_t localGoNr, const char* name, const Dpt& dpt, const KNXValue& value, bool forceSend)
{
    GroupObject& go = knx.getGroupObject(localGoNr + goOffset);
    if (forceSend || (u_int64_t) go.value(dpt) != (u_int64_t) value)
    {
        Serial.print("Use go ");
        Serial.print(go.asap());
        Serial.print(" - Forced: ");
        Serial.print(forceSend);
        Serial.print(" ");
        go.value(value, dpt);
        logValue(name, "set", value);
        return true;
    }
    return false;
}

void Component::goSetWithoutSend(uint16_t localGoNr, const char* name, const Dpt& dpt, const KNXValue& value)
{
    GroupObject& go = knx.getGroupObject(localGoNr + goOffset);

    Serial.print("Use go ");
    Serial.print(go.asap());
    Serial.print(" - No Send -");
    go.valueNoSend(value, dpt);
    logValue(name, "set", value);
}

void Component::goSetHandleSendMode(uint16_t localGoNr, const char* name, const Dpt& dpt, bool value, bool forceSend, OutputSendMode sendMode, bool locked)
{
      switch (sendMode)
     {
          case OutputSendMode::OnAndOff:
               break;
          case OutputSendMode::OnlyOn:
               if (!value)
                    return;
               break;
          case OutputSendMode::OnlyOff:
               if (value)
                    return;
               break;
          case OutputSendMode::InvertedOnAndOff:
               if (!locked)
                    value = !value;
               break;
          case OutputSendMode::InvertedOnlyOnAfterInversion:
               if (!locked)
                    value = !value;
               if (!value)
                    return;
               break;
          case OutputSendMode::InvertedOnlyOffAfterInversion:
               if (!locked)
                    value = !value;
               if (value)
                    return;
               break;
     }
     goSet(localGoNr, name, dpt, value, forceSend);
}

const KNXValue Component::goGet(uint16_t localGoNr, const char* name, const Dpt& dpt)
{
    return knx.getGroupObject(localGoNr + goOffset).value(dpt);
}

void Component::goSendReadRequest(uint16_t localGoNr, const char* name, const Dpt& dpte)
{
    GroupObject& go = knx.getGroupObject(localGoNr + goOffset);
    Serial.print("Use go ");
    Serial.print(go.asap());
    Serial.println(" - Read");
    go.requestObjectRead();
}

void Component::loop(unsigned long now, bool initalize)
{
}   
void Component::received(GroupObject& groupObject)
{
}
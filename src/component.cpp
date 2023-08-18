 #include "component.h"

Component::Component(const char* componentName)
    : componentName(componentName)
{
}

const char* Component::getName()
{
    return componentName;
}

void Component::readKnxParameterString(const char* operation, uint8_t* parameter, char* buffer, size_t chars)
{
    Component::readKnxParameterString(componentName, operation, parameter, buffer, chars);
}

void Component::readKnxParameterString(const char* name, const char* operation, uint8_t* parameter, char* buffer, size_t bufferSize)
{
    size_t chars = bufferSize - 1;
    char* result = (char*) parameter;
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
        ArduinoPlatform::SerialDebug->print(componentName);
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

bool Component::isGo(GroupObject& groupObject, GroupObject& go)
{
    return groupObject.asap() == go.asap();
}

bool Component::isGo(GroupObject& groupObject, GroupObject& go, const Dpt& dpt)
{
    return isGo(groupObject, go);
}

bool Component::goSet(GroupObject& go, const Dpt& dpt, const KNXValue& value, bool forceSend)
{
    if (forceSend || (u_int64_t) go.value(dpt) != (u_int64_t) value)
    {
        Serial.print("Use go ");
        Serial.print(go.asap());
        Serial.print(" - Forced: ");
        Serial.print(forceSend);
        Serial.print(" ");
        go.value(value, dpt);
        logValue(componentName, "set", value);
        return true;
    }
    return false;
}

void Component::goSetWithoutSend(GroupObject& go, const Dpt& dpt, const KNXValue& value)
{
    Serial.print("Use go ");
    Serial.print(go.asap());
    Serial.print(" - No Send -");
    go.valueNoSend(value, dpt);
    logValue(componentName, "set", value);
}

void Component::goSetHandleSendMode(GroupObject& go, const Dpt& dpt, bool value, bool forceSend, OutputSendMode sendMode, bool locked)
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
     goSet(go, dpt, value, forceSend);
}

const KNXValue Component::goGet(GroupObject& go, const Dpt& dpt)
{
    return go.value(dpt);
}

void Component::goSendReadRequest(GroupObject& go, const Dpt& dpte)
{
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
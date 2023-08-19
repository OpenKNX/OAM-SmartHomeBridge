 #include "component.h"

Component::Component(const char* componentName)
    : componentName(componentName)
{
}

const char* Component::getName()
{
    return componentName;
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

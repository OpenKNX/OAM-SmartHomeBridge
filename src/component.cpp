 #include "component.h"
 #include "OpenKNX.h"

Component::Component(const char* componentName)
    : componentName(componentName)
{
}

const std::string Component::logPrefix()
{
    return std::string(componentName);
}

const char* Component::getName()
{
    return componentName;
}

bool Component::isKo(GroupObject& ko, GroupObject& koCompare)
{
    return ko.asap() == koCompare.asap();
}

bool Component::isKo(GroupObject& ko, GroupObject& koCompare, const Dpt& dpt)
{
    return isKo(ko, koCompare);
}

bool Component::koSet(GroupObject& ko, const Dpt& dpt, const KNXValue& value, bool forceSend)
{
    if (forceSend || (u_int64_t) ko.value(dpt) != (u_int64_t) value)
    {
        if (forceSend)
            logInfoP("[%s] Send ko %d: %f (forced)", componentName, ko.asap(), (float) value);
        else
            logInfoP("[%s] Send ko %d: %f", componentName, ko.asap(), (float) value);    
        ko.value(value, dpt);
        return true;
    }
    return false;
}

void Component::koSetWithoutSend(GroupObject& ko, const Dpt& dpt, const KNXValue& value)
{
    logInfoP("[%s] Set ko %d: %f (no send)", componentName, ko.asap(), (float) value);
    ko.valueNoSend(value, dpt);
}

const KNXValue Component::koGet(GroupObject& ko, const Dpt& dpt)
{
    return ko.value(dpt);
}

void Component::koSendReadRequest(GroupObject& ko, const Dpt& dpte)
{
    logInfoP("[%s] Read request for ko %d", componentName, ko.asap());
    ko.requestObjectRead();
}

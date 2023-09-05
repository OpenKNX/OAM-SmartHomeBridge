 #include "component.h"
 #include "OpenKNX.h"

Component::Component(const char* componentName)
    : componentName(componentName)
{
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
            logInfoP("Send ko %d: %f (forced)", ko.asap(), (float) value);
        else
            logInfoP("Send ko %d: %f", ko.asap(), (float) value);    
        ko.value(value, dpt);
        return true;
    }
    return false;
}

void Component::koSetWithoutSend(GroupObject& ko, const Dpt& dpt, const KNXValue& value)
{
    logDebugP("Set ko %d: %f (no send)", ko.asap(), (float) value);
    ko.valueNoSend(value, dpt);
}

const KNXValue Component::koGet(GroupObject& ko, const Dpt& dpt)
{
    return ko.value(dpt);
}

void Component::koSendReadRequest(GroupObject& ko, const Dpt& dpte)
{
    logInfoP("Read request for ko %d", ko.asap());
    ko.requestObjectRead();
}

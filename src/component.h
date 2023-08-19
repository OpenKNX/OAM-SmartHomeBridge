#pragma once
#include <arduino.h>
#include <knx.h> 
#include <list>

class Component
{
        const char* componentName;  
    
    protected:
        bool goSet(GroupObject& go, const Dpt& dpt, const KNXValue& value, bool forceSend);
        void goSetWithoutSend(GroupObject& go, const Dpt& dpt, const KNXValue& value);
        const KNXValue goGet(GroupObject& go, const Dpt& type);
        void goSendReadRequest(GroupObject& go, const Dpt& dpt);
        bool isGo(GroupObject& groupObject, GroupObject& go);
        bool isGo(GroupObject& groupObject, GroupObject& go, const Dpt& type);

        void logValue(const char* goName, const char* operation, float value);
 
    public:
        const char* getName();
        Component(const char* componentName);
 
};

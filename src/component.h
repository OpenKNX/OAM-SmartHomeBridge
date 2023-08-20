#pragma once
#include <arduino.h>
#include <knx.h> 
#include <list>

class Component
{
        const char* componentName;    
    protected:
        bool koSet(GroupObject& ko, const Dpt& dpt, const KNXValue& value, bool forceSend);
        void koSetWithoutSend(GroupObject& ko, const Dpt& dpt, const KNXValue& value);
        const KNXValue koGet(GroupObject& ko, const Dpt& type);
        void koSendReadRequest(GroupObject& ko, const Dpt& dpt);
        bool isKo(GroupObject& ko, GroupObject& koCompare);
        bool isKo(GroupObject& ko, GroupObject& koCompare, const Dpt& type); 
        virtual const std::string logPrefix();
    public:
        const char* getName();
        Component(const char* componentName);
};

#pragma once
#include <arduino.h>
#include <knx.h> 
#include <list>
#include "globalEnums.h"

class Component
{
        const char* componentName;  
        void readKnxParameterString(const char* operation, uint8_t* parameter, char* buffer, size_t chars);
        static void readKnxParameterString(const char* name, const char* operation, uint8_t* parameter, char* buffer, size_t chars);
    
    protected:
        bool goSet(GroupObject& go, const Dpt& dpt, const KNXValue& value, bool forceSend);
        void goSetHandleSendMode(GroupObject& go, const Dpt& dpt, bool value, bool forceSend, OutputSendMode sendMode, bool locked);
        void goSetWithoutSend(GroupObject& go, const Dpt& dpt, const KNXValue& value);
        const KNXValue goGet(GroupObject& go, const Dpt& type);
        void goSendReadRequest(GroupObject& go, const Dpt& dpt);
        bool isGo(GroupObject& groupObject, GroupObject& go);
        bool isGo(GroupObject& groupObject, GroupObject& go, const Dpt& type);

     
        void logValue(const char* goName, const char* operation, float value);
 
    public:
        const char* getName();
        virtual void loop(unsigned long now, bool initalize);
        virtual void received(GroupObject& groupObject);
        Component(const char* componentName);
 
};

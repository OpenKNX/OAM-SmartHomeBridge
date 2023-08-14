#pragma once
#include <arduino.h>
#include <knx.h> 
#include <list>
#include "globalEnums.h"

class Component
{
    protected:
        const char* componentName;
        bool goSet(GroupObject& go, const Dpt& dpt, const KNXValue& value, bool forceSend);
        void goSetHandleSendMode(GroupObject& go, const Dpt& dpt, bool value, bool forceSend, OutputSendMode sendMode, bool locked);
        void goSetWithoutSend(GroupObject& go, const Dpt& dpt, const KNXValue& value);
        const KNXValue goGet(GroupObject& go, const Dpt& type);
        void goSendReadRequest(GroupObject& go, const Dpt& dpt);
        bool isGo(GroupObject& groupObject, GroupObject& go);
        bool isGo(GroupObject& groupObject, GroupObject& go, const Dpt& type);

        uint8_t readKnxParameterUInt8(const char* operation, uint32_t parameterAddress);
        uint32_t readKnxParameterUInt32(const char* operation, uint32_t parameterAddress);
        float readKnxParameterFloat(const char* operation, uint32_t parameterAddress);
        void readKnxParameterString(const char* operation, uint8_t* parameter, char* buffer, size_t chars);
     
        void logValue(const char* goName, const char* operation, float value);
 
    public:
        virtual void loop(unsigned long now, bool initalize);
        virtual void received(GroupObject& groupObject);
        static void readKnxParameterString(const char* name, const char* operation, uint8_t* parameter, char* buffer, size_t chars);
        Component(const char* componentName);
 
};

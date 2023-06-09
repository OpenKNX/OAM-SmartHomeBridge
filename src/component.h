#pragma once
#include <arduino.h>
#include <knx.h> 
#include <list>
#include "globalEnums.h"



class Component
{
    private:
        static std::list<Component*> instances;   
        
    protected:
        const char* name;
        const uint16_t goOffset;

        bool goSet(uint16_t localGoNr, const char* name, const Dpt& type, const KNXValue& value, bool forceSend);
        void goSetHandleSendMode(uint16_t localGoNr, const char* name, const Dpt& dpt, bool value, bool forceSend, OutputSendMode sendMode, bool locked);
        void goSetWithoutSend(uint16_t localGoNr, const char* name, const Dpt& dpt, const KNXValue& value);
        const KNXValue goGet(uint16_t localGoNr, const char* name, const Dpt& type);
        void goSendReadRequest(uint16_t localGoNr, const char* name, const Dpt& dpte);
        bool isGo(GroupObject& groupObject, uint16_t localGoNr, const char* name, const Dpt& type);

        uint8_t readKnxParameterUInt8(const char* operation, uint32_t parameterAddress);
        uint32_t readKnxParameterUInt32(const char* operation, uint32_t parameterAddress);
        float readKnxParameterFloat(const char* operation, uint32_t parameterAddress);
        void readKnxParameterString(const char* operation, uint32_t parameterAddress, char* buffer, size_t chars);
     
        void logValue(const char* goName, const char* operation, float value);
        virtual void loop(unsigned long now, bool initalize);
        virtual void received(GroupObject& groupObject);
 
    public:
        static void readKnxParameterString(const char* name, const char* operation, uint32_t parameterAddress, char* buffer, size_t chars);
        static void loopAll(unsigned long now, bool initalize);
        static void receiveAll(GroupObject& groupObject);
        Component(const char* name, uint16_t goOffset);
        ~Component();
 
};

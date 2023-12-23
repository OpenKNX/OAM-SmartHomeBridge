#pragma once
#include <arduino.h>
#include <knx.h> 
#include <vector>

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
        virtual const std::string logPrefix() = 0;
    public:
        const char* getName();
        Component(const char* componentName);

        static void* operator new(size_t size)
        {
            void *storage = ps_malloc(size);
            return storage;
        }
};

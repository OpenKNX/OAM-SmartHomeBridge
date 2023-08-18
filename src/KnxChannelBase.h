#pragma once
#include "component.h"
#include "OpenKNX.h"

class KnxChannelBase : public OpenKNX::Channel, public Component
{
    private:
        const char* utf8Name = NULL;
    public:
        ~KnxChannelBase();
        KnxChannelBase(uint16_t channelIndex);
        virtual const std::string name();
        const char* getNameInUTF8();
};
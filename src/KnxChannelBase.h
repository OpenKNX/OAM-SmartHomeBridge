#pragma once
#include "component.h"
#include "OpenKNX.h"


class KnxChannelBase : public OpenKNX::Channel, public Component
{
    public:
        char deviceName[25 + 1]; // One more then chars for ending 0
        KnxChannelBase(uint16_t channelIndex);
        virtual const std::string name();
};
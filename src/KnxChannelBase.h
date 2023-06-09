#pragma once
#include "component.h"

class KnxChannelBase : public Component
{
    public:
        char deviceName[25 + 1]; // One more then chars for ending 0
        KnxChannelBase(uint16_t goOffset, uint32_t parameterOffet);
};
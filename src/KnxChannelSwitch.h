#pragma once
#include "KnxChannelBase.h"

class KnxChannelSwitch;

class ISwitchBridge
{
    public:
    virtual void initialize(KnxChannelSwitch* switchDevice) = 0;
    virtual void setPower(bool on) = 0;
};

class KnxChannelSwitch : public KnxChannelBase
{
    public:
        std::list<ISwitchBridge*>* switchBridges; 
        KnxChannelSwitch(std::list<ISwitchBridge*>* switchBridges, uint16_t channelIndex);
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandPower(ISwitchBridge* switchBridge, bool on);
};

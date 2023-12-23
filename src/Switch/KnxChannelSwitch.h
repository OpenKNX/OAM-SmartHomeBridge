#pragma once
#include "KnxChannelBase.h"

class KnxChannelSwitch;

class SwitchBridge : public ChannelBridgeBase<KnxChannelSwitch>
{
public:
    virtual void setPower(bool on) = 0;
};

class KnxChannelSwitch : public KnxChannelBase
{
    public:
        std::vector<SwitchBridge*>* switchBridges; 
        KnxChannelSwitch(std::vector<SwitchBridge*>* switchBridges, uint16_t channelIndex);
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandPower(SwitchBridge* switchBridge, bool on);
        virtual const std::string name() override;
};

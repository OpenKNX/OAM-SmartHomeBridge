#pragma once
#include "KnxChannelBase.h"

class KnxChannelFan;

class FanBridge : public ChannelBridgeBase<KnxChannelFan>
{
public:
    virtual void setAutomatic(bool automatic) = 0;
    virtual void setPower(bool on) = 0;
};

class KnxChannelFan : public KnxChannelBase
{
    public:
        std::vector<FanBridge*>* fanBridges; 
        KnxChannelFan(std::vector<FanBridge*>* fanBridges, uint16_t channelIndex);
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandPower(FanBridge* fanBridge, bool on);
        void commandAutomatic(FanBridge* fanBridge, bool on);
        virtual const std::string name() override;
};

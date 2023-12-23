#pragma once
#include "KnxChannelBase.h"

class KnxChannelDimmer;

class DimmerBridge : public ChannelBridgeBase<KnxChannelDimmer>
{
public:
    virtual void setBrightness(uint8_t brightness) = 0;
};

class KnxChannelDimmer : public KnxChannelBase
{
    public:
        std::vector<DimmerBridge *> *dimmerBridges;
        KnxChannelDimmer(std::vector<DimmerBridge *> *dimmerBridges, uint16_t channelIndex);
    protected:
        uint8_t lastBrighness = 100;
        uint8_t lastBrighnessLessThan100 = 50;
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandPower(DimmerBridge* dimmerBridge, bool on);
        void commandBrightness(DimmerBridge* dimmerBridge, uint8_t brightness);
        virtual const std::string name() override;
};

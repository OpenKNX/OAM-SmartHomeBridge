#pragma once
#include "KnxChannelBase.h"

class KnxChannelDimmer;

class IDimmerBridge
{
    public:
    virtual void initialize(KnxChannelDimmer* dimmerDevice) = 0;
    virtual void setBrightness(uint8_t brightness) = 0;
};

class KnxChannelDimmer : public KnxChannelBase
{
    public:
        std::list<IDimmerBridge *> *dimmerBridges;
        KnxChannelDimmer(std::list<IDimmerBridge *> *dimmerBridges, uint16_t channelIndex);
    protected:
        uint8_t lastBrighness = 100;
        uint8_t lastBrighnessLessThan100 = 50;
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandPower(IDimmerBridge* dimmerBridge, bool on);
        void commandBrightness(IDimmerBridge* dimmerBridge, uint8_t brightness);
};

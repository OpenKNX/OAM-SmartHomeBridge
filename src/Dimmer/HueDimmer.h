#pragma once
#include "HueBridge.h"
#include "KnxChannelDimmer.h"

class HueDimmer : public DimmerBridge
{
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;

public:
    HueDimmer(HueBridge* bridge);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setBrightness(uint8_t brightness) override;
};
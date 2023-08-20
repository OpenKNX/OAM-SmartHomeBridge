#pragma once
#include "HueBridge.h"
#include "KnxChannelDimmer.h"

class HueDimmer : public DimmerBridge
{
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;

public:
    HueDimmer(HueBridge* bridge);
    virtual void setup() override;

    boolean update();
    virtual void setBrightness(uint8_t brightness) override;
};
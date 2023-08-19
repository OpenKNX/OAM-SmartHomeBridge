#pragma once
#include "HueBridge.h"
#include "KnxChannelDimmer.h"

class HueDimmer : public IDimmerBridge
{
    KnxChannelDimmer *dimmerDevice;
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;

public:
    HueDimmer(HueBridge* bridge);
    virtual void initialize(KnxChannelDimmer *dimmerDevice) override;

    boolean update();
    virtual void setBrightness(uint8_t brightness) override;
};
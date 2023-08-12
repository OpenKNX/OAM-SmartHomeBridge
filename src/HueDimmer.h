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
    void initialize(KnxChannelDimmer *dimmerDevice);

    boolean update();
    virtual void setBrightness(uint8_t brightness);
};
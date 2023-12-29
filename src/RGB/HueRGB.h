#pragma once
#include "HueBridge.h"
#include "KnxChannelRGB.h"

class HueRGB : public RGBBridge
{
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;

public:
    HueRGB(HueBridge* bridge);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setRGB(uint32_t rgb) override;
    virtual void setPower(bool on) override;
};
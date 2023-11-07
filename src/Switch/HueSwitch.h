#pragma once
#include "HueBridge.h"
#include "KnxChannelSwitch.h"

class HueSwitch : public SwitchBridge
{
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;
public:
    HueSwitch(HueBridge* bridge);
    virtual void setup(uint8_t _channelIndex) override;
    boolean update();
    virtual void setPower(bool on) override;
};
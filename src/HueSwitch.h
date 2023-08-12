#pragma once
#include "HueBridge.h"
#include "KnxChannelSwitch.h"

class HueSwitch : public ISwitchBridge
{
    KnxChannelSwitch *switchDevice;
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;

public:
    HueSwitch(HueBridge* bridge);
    void initialize(KnxChannelSwitch *switchDevice);
    boolean update();
    virtual void setPower(bool on);
};
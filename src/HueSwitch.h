#pragma once
#include "HueBridge.h"
#include "KnxChannelSwitch.h"

class HueSwitch : public ISwitchInterface
{
    KnxChannelSwitch *SwitchDevice;
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;

public:
    HueSwitch(HueBridge* bridge);
    void initialize(KnxChannelSwitch *SwitchDevice);

    boolean update();
    virtual bool getPower();
    virtual void setPower(bool value);
};
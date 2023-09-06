#pragma once
#include "HueBridge.h"
#include "KnxChannelFan.h"

class HueFan : public FanBridge
{
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;
public:
    HueFan(HueBridge* bridge);
    virtual void setup(uint8_t _channelIndex) override;
    boolean update();
    virtual void setPower(bool on) override;
    virtual void setAutomatic(bool automatic) override;
};
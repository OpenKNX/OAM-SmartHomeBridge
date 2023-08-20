#pragma once
#include "HueBridge.h"
#include "KnxChannelRolladen.h"

class HueRolladen : public RolladenBridge
{
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;

public:
    HueRolladen(HueBridge* bridge);
    virtual void setup() override;

    virtual boolean update();
    virtual void setPosition(uint8_t position) override;
    virtual void setMovement(MoveState movement) override;
};
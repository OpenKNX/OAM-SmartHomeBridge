#pragma once
#include "HueBridge.h"
#include "KnxChannelRolladen.h"

class HueRolladen : public IRolladenBridge
{
    KnxChannelRolladen *rolladenDevice;
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;

public:
    HueRolladen(HueBridge* bridge);
    void initialize(KnxChannelRolladen *rolladenDevice);

    virtual boolean update();
    virtual void setPosition(uint8_t position) override;
    virtual void setMovement(MoveState movement) override;
};
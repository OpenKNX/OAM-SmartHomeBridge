#pragma once
#include "HueBridge.h"
#include "KnxChannelDoorWindow.h"

class HueDoorWindow : public DoorWindowBridge
{
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;

public:
    HueDoorWindow(HueBridge* bridge);
    virtual void setup(uint8_t _channelIndex) override;

    virtual boolean update();
    virtual void setPosition(uint8_t position) override;
    virtual void setMovement(DoorWindowMoveState movement) override;
    virtual void setObstructionDetected(bool obstructionDetected) override;
};
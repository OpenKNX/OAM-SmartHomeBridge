#pragma once
#include "HueBridge.h"
#include "KnxChannelJalousie.h"
#include "HueRolladen.h"


class HueJalousie : public HueRolladen, public IJalousieBridge
{
protected:
    virtual void initialize(KnxChannelRolladen *rolladenDevice); // Needed for interface implementation of base. Do not call this function.
public:
    HueJalousie(HueBridge* bridge);
    void initialize(KnxChannelJalousie *jalousieDevice);
 
    virtual boolean update() override;
    virtual void setPosition(uint8_t position) override;
    virtual void setMovement(MoveState movement) override;
    virtual void setSlatPosition(uint8_t slatPosition) override;
};
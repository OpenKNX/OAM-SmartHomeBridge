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
 
    virtual boolean update();
    virtual void setPosition(uint8_t position);
    virtual void setMovement(MoveState movement);
    virtual void setSlatPosition(uint8_t slatPosition);
};
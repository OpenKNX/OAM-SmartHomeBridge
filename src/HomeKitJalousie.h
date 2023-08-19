#pragma once
#include "HomeSpan.h"
#include "KnxChannelJalousie.h"
#include "HomeKitRolladen.h"

class HomeKitJalousie : public HomeKitRolladen, public IJalousieBridge
{
private:
    Characteristic::CurrentHorizontalTiltAngle *currentHorizontalTiltAngle;
    Characteristic::TargetHorizontalTiltAngle *targetHorizontalTiltAngle;
protected:
    virtual void initialize(KnxChannelRolladen *rolladenDevice) override; // Needed for interface implementation of base. Do not call this function.
public:
    HomeKitJalousie(int device);
    void initialize(KnxChannelJalousie *jalousieDevice) override;
 
    virtual boolean update();
    virtual void setPosition(uint8_t position) override;
    virtual void setMovement(MoveState movement) override;
    virtual void setSlatPosition(uint8_t slatPosition) override;
};
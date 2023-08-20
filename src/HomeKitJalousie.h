#pragma once
#include "HomeSpan.h"
#include "KnxChannelJalousie.h"
#include "HomeKitRolladen.h"

class HomeKitJalousie : public HomeKitRolladen
{
private:
    Characteristic::CurrentHorizontalTiltAngle *currentHorizontalTiltAngle;
    Characteristic::TargetHorizontalTiltAngle *targetHorizontalTiltAngle;
public:
    HomeKitJalousie(int device);
    virtual void setup() override;
 
    virtual boolean update() override;
    virtual void setSlatPosition(uint8_t slatPosition) override;
};
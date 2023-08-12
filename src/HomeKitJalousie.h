#pragma once
#include "HomeSpan.h"
#include "KnxChannelJalousie.h"
#include "HomeKitRolladen.h"

class HomeKitJalousie : public HomeKitRolladen, public IJalousieBridge
{
    Characteristic::CurrentHorizontalTiltAngle *currentHorizontalTiltAngle;
    Characteristic::TargetHorizontalTiltAngle *targetHorizontalTiltAngle;

public:
    HomeKitJalousie(int device);
    void initialize(KnxChannelJalousie *jalousieDevice);

    virtual boolean update();
    virtual void setSlatPosition(uint8_t slatPosition) = 0;
};
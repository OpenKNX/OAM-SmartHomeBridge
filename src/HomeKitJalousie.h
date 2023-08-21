#pragma once
#include "HomeSpan.h"
#include "KnxChannelJalousie.h"
#include "HomeKitRolladen.h"

class HomeKitJalousie : public HomeKitRolladen
{
    const static int NoReceivedTargetAngle = 255;
private:
    Characteristic::CurrentHorizontalTiltAngle *currentHorizontalTiltAngle;
    Characteristic::TargetHorizontalTiltAngle *targetHorizontalTiltAngle;
    int receivedTargetAngle = NoReceivedTargetAngle;
public:
    HomeKitJalousie(int device);
    virtual void setup() override;
 
    virtual boolean update() override;
    virtual void setSlatPosition(uint8_t slatPosition) override;
    virtual void setMovement(MoveState movement) override;
};
#include "HomeKitJalousie.h"

HomeKitJalousie::HomeKitJalousie(int device) :
    HomeKitRolladen(device)
{
}

void HomeKitJalousie::initialize(KnxChannelRolladen *rolladenDevice)
{
    // Do nothing, the function below will be called to initialize the base.
}

void HomeKitJalousie::initialize(KnxChannelJalousie *jalousieDevice)
{
    HomeKitRolladen::initialize(jalousieDevice);
       currentHorizontalTiltAngle = new Characteristic::CurrentHorizontalTiltAngle(100);
    currentHorizontalTiltAngle->setRange(0, 100, 10);
       targetHorizontalTiltAngle = new Characteristic::TargetHorizontalTiltAngle(100);
    targetHorizontalTiltAngle->setRange(0, 100, 10);
}

boolean HomeKitJalousie::update()
{
    HomeKitRolladen::update();
    
    if (targetHorizontalTiltAngle->updated())
    {
        ((KnxChannelJalousie*)rolladenDevice)->commandSlatPosition(this, 100 - targetHorizontalTiltAngle->getNewVal());
    }
    return true;
}

void HomeKitJalousie::setPosition(uint8_t position)
{
    HomeKitRolladen::setPosition(position);
}

void HomeKitJalousie::setMovement(MoveState movement)
{
    HomeKitRolladen::setMovement(movement);
}

void HomeKitJalousie::setSlatPosition(uint8_t position)
{
    Serial.print("Slat position ");
    Serial.println(position);
    if (position < 0)
        position = 0;
    if (position > 100)
        position = 100;
    currentHorizontalTiltAngle->setVal(100 - position);
}


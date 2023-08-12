#include "HomeKitJalousie.h"

HomeKitJalousie::HomeKitJalousie(int device) :
    HomeKitRolladen(device)
{
}

void HomeKitJalousie::initialize(KnxChannelJalousie *jalousieDevice)
{
    HomeKitRolladen::initialize(jalousieDevice);
       currentHorizontalTiltAngle = new Characteristic::CurrentHorizontalTiltAngle();
       targetHorizontalTiltAngle = new Characteristic::TargetHorizontalTiltAngle();
}

boolean HomeKitJalousie::update()
{
    if (targetHorizontalTiltAngle->getVal() != targetHorizontalTiltAngle->getNewVal())
        ((KnxChannelJalousie*)rolladenDevice)->commandSlatPosition(this, targetHorizontalTiltAngle->getNewVal());
    return true;
}


void HomeKitJalousie::setSlatPosition(uint8_t position)
{
    currentHorizontalTiltAngle->setVal(position);
}


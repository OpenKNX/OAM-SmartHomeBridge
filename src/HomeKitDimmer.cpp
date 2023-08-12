#include "HomeKitDimmer.h"

HomeKitDimmer::HomeKitDimmer(int device) :
    device(device)
{
}

void HomeKitDimmer::initialize(KnxChannelDimmer *dimmerDevice)
{
    this->dimmerDevice = dimmerDevice;
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(dimmerDevice->deviceName);
    new ServiceImplementation(this);
       power = new Characteristic::On();
    level = new Characteristic::Brightness(100);
    level->setRange(1, 100, 1);
}

boolean HomeKitDimmer::update()
{
    if (level->getVal() != level->getNewVal())
        dimmerDevice->commandBrightness(this, level->getNewVal());
    if (power->getVal() != power->getNewVal())
        dimmerDevice->commandPower(this, power->getNewVal());
    return true;
}


void HomeKitDimmer::setBrightness(uint8_t brightness)
{
    if (brightness == 0)
    {
        power->setVal(false);
    }
    else
    {
        level->setVal(brightness);
        power->setVal(true);
    }
}

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
        new Characteristic::Name(dimmerDevice->getNameInUTF8());
    new ServiceImplementation(this);
        power = new Characteristic::On();
        level = new Characteristic::Brightness();
}

boolean HomeKitDimmer::update()
{
    if (level->updated())
        dimmerDevice->commandBrightness(this, level->getNewVal());
    if (power->updated())
        dimmerDevice->commandPower(this, power->getNewVal());
    return true;
}


void HomeKitDimmer::setBrightness(uint8_t brightness)
{
    if (brightness < 0)
        brightness = 0;
    if (brightness > 100)
        brightness = 100;

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

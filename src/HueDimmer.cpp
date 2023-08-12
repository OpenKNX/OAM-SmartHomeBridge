#include "HueDimmer.h"

HueDimmer::HueDimmer(HueBridge* hueBridge)
: hueBridge(hueBridge)
{
    
}

void HueDimmer::initialize(KnxChannelDimmer *dimmerDevice)
{
    this->dimmerDevice = dimmerDevice;
    espalexaDevice = new EspalexaDevice(dimmerDevice->deviceName, [this](EspalexaDevice* d){update();}, EspalexaDeviceType::dimmable, 255);
    espalexaDevice->setState(false);
    hueBridge->espalexa.addDevice(espalexaDevice);
}

boolean HueDimmer::update()
{
    switch (espalexaDevice->getLastChangedProperty())
    {
        case EspalexaDeviceProperty::on:
            dimmerDevice->commandPower(this, true);
            break;
        case EspalexaDeviceProperty::off:
            dimmerDevice->commandPower(this, false);
            break;
         case EspalexaDeviceProperty::bri:
            dimmerDevice->commandBrightness(this, espalexaDevice->getPercent());
            break;
    }
    return (true);
}

void HueDimmer::setBrightness(uint8_t brightness)
{
    if (brightness == 0)
    {
        espalexaDevice->setState(false);
    }
    else
    {
        espalexaDevice->setPercent(brightness);
        espalexaDevice->setState(true);
    }
}

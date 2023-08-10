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
    dimmerDevice->deviceChanged(this);
    return (true);
}

bool HueDimmer::getPower()
{
    return espalexaDevice->getState();
}

void HueDimmer::setPower(bool value)
{
    espalexaDevice->setState(value);
}

int HueDimmer::getBrightness()
{
    return espalexaDevice->getPercent();
}

void HueDimmer::setBrightness(int brightness)
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

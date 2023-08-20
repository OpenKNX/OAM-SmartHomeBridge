#include "HueDimmer.h"

HueDimmer::HueDimmer(HueBridge* hueBridge)
: hueBridge(hueBridge)
{
    
}

void HueDimmer::setup()
{
    espalexaDevice = new EspalexaDevice(_channel->getNameInUTF8(), [this](EspalexaDevice* d){update();}, EspalexaDeviceType::dimmable, 255);
    espalexaDevice->setState(false);
    hueBridge->espalexa.addDevice(espalexaDevice);
}

boolean HueDimmer::update()
{
    switch (espalexaDevice->getLastChangedProperty())
    {
        case EspalexaDeviceProperty::on:
            _channel->commandPower(this, true);
            break;
        case EspalexaDeviceProperty::off:
            _channel->commandPower(this, false);
            break;
         case EspalexaDeviceProperty::bri:
            _channel->commandBrightness(this, espalexaDevice->getPercent());
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

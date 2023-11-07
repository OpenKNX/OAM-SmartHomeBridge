#include "HueSwitch.h"

HueSwitch::HueSwitch(HueBridge* hueBridge)
: hueBridge(hueBridge)
{
    
}

void HueSwitch::setup(uint8_t _channelIndex)
{
    espalexaDevice = new EspalexaDevice(_channel->getNameInUTF8(), [this](EspalexaDevice* d){update();}, EspalexaDeviceType::onoff);
    espalexaDevice->setState(false);
    hueBridge->espalexa.addDevice(_channel->channelIndex(), espalexaDevice);
}

boolean HueSwitch::update()
{
    switch (espalexaDevice->getLastChangedProperty())
    {
        case EspalexaDeviceProperty::on:
            _channel->commandPower(this, true);
            break;
        case EspalexaDeviceProperty::off:
            _channel->commandPower(this, false);
            break;
    }
    return true;
}

void HueSwitch::setPower(bool value)
{
    espalexaDevice->setState(value);
}


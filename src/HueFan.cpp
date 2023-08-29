#include "HueFan.h"

HueFan::HueFan(HueBridge* hueBridge)
: hueBridge(hueBridge)
{
    
}

void HueFan::setup()
{
    espalexaDevice = new EspalexaDevice(_channel->getNameInUTF8(), [this](EspalexaDevice* d){update();}, EspalexaDeviceType::onoff);
    espalexaDevice->setState(false);
    hueBridge->espalexa.addDevice(_channel->channelIndex(), espalexaDevice);
}

boolean HueFan::update()
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

void HueFan::setPower(bool power)
{
    espalexaDevice->setState(power);
}

void HueFan::setAutomatic(bool automatic)
{

}



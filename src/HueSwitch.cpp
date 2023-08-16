#include "HueSwitch.h"

HueSwitch::HueSwitch(HueBridge* hueBridge)
: hueBridge(hueBridge)
{
    
}

void HueSwitch::initialize(KnxChannelSwitch *switchDevice)
{
    this->switchDevice = switchDevice;
    espalexaDevice = new EspalexaDevice(switchDevice->deviceName, [this](EspalexaDevice* d){update();}, EspalexaDeviceType::onoff);
    espalexaDevice->setState(false);
    hueBridge->espalexa.addDevice(espalexaDevice);
}

boolean HueSwitch::update()
{
    switch (espalexaDevice->getLastChangedProperty())
    {
        case EspalexaDeviceProperty::on:
            switchDevice->commandPower(this, true);
            break;
        case EspalexaDeviceProperty::off:
            switchDevice->commandPower(this, false);
            break;
    }
    return true;
}

void HueSwitch::setPower(bool value)
{
    espalexaDevice->setState(value);
}


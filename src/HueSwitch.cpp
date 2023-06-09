#include "HueSwitch.h"

HueSwitch::HueSwitch(HueBridge* hueBridge)
: hueBridge(hueBridge)
{
    
}

void HueSwitch::initialize(KnxChannelSwitch *SwitchDevice)
{
    this->SwitchDevice = SwitchDevice;
    espalexaDevice = new EspalexaDevice(SwitchDevice->deviceName, [this](EspalexaDevice* d){update();}, EspalexaDeviceType::onoff);
    espalexaDevice->setState(false);
    hueBridge->espalexa.addDevice(espalexaDevice);
}

boolean HueSwitch::update()
{
    SwitchDevice->deviceChanged(this);
    return (true);
}

bool HueSwitch::getPower()
{
    return espalexaDevice->getState();
}

void HueSwitch::setPower(bool value)
{
    espalexaDevice->setState(value);
}


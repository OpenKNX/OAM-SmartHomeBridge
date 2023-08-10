#include "HomeKitSwitch.h"

HomeKitSwitch::HomeKitSwitch(int device) :
    device(device)
{
}

void HomeKitSwitch::initialize(KnxChannelSwitch *switchDevice)
{
    this->switchDevice = switchDevice;
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(switchDevice->deviceName);
    new ServiceImplementation(this);
       power = new Characteristic::On();
}

boolean HomeKitSwitch::update()
{
    switchDevice->deviceChanged(this);
    return (true);
}

bool HomeKitSwitch::getPower()
{
    return power->getNewVal();
}

void HomeKitSwitch::setPower(bool value)
{
    power->setVal(value);
}

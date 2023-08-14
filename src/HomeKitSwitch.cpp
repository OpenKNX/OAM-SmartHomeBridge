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
    if (power->updated())
        switchDevice->commandPower(this, power->getNewVal());
    return (true);
}

void HomeKitSwitch::setPower(bool value)
{
    power->setVal(value);
}

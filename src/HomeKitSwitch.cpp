#include "HomeKitSwitch.h"

HomeKitSwitch::HomeKitSwitch(int device) :
    device(device)
{
}


void HomeKitSwitch::setup()
{
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
    new ServiceImplementation(this);
       power = new Characteristic::On();
}

boolean HomeKitSwitch::update()
{
    if (power->updated())
        _channel->commandPower(this, power->getNewVal());
    return (true);
}

void HomeKitSwitch::setPower(bool value)
{
    power->setVal(value);
}

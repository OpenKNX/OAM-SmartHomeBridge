#include "HomeKitSwitch.h"

HomeKitSwitch::HomeKitSwitch(int device) :
    device(device)
{
}


void HomeKitSwitch::setup(uint8_t _channelIndex)
{
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
    switch (ParamBRI_CHDeviceType)
    {
        case 10: 
          new ServiceImplementationSwitch(this);
          break;
        case 11: 
          new ServiceImplementationOutlet(this);
          break;
        case 20:
          new ServiceImplementationLightBulb(this);
          break;
    }
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

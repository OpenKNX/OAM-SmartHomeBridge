#include "HomeKitFan.h"

HomeKitFan::HomeKitFan(int device) :
    device(device)
{
}


void HomeKitFan::setup(uint8_t _channelIndex)
{  
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
    new ServiceImplementation(this);
        active = new Characteristic::Active();
        if (ParamBRI_CHFanAutomatic != 0)
        {
            currentFanState = new Characteristic::CurrentFanState(0);
            targetFanState = new Characteristic::TargetFanState(0);
        }
}

boolean HomeKitFan::update()
{
    if (active->updated())
    {
        _channel->commandPower(this, active->getNewVal());
        return true;
    }
    if (targetFanState != nullptr && targetFanState->updated())
    {
        _channel->commandAutomatic(this, targetFanState->getNewVal() == 1);
        return true;
    }
    return false;
}

void HomeKitFan::setPower(bool value)
{
    if (value)
    {
        if (currentFanState != nullptr)
            currentFanState->setVal(2); // active
        active->setVal(1);
    }
    else
    {
        if (currentFanState != nullptr)
        {
            if (automatic)
                currentFanState->setVal(1); // idle
            else
                currentFanState->setVal(0); // inactive
        }
        active->setVal(0);
    }
}

void HomeKitFan::setAutomatic(bool automatic)
{
    this->automatic = automatic;
    if (automatic)
    {
        if (targetFanState != nullptr)
            targetFanState->setVal(1); // automatic
    }
    else
    {
        if (targetFanState != nullptr)
            targetFanState->setVal(0); // manual
        if (currentFanState != nullptr)
        {
            if (active->getVal() == 0)
                currentFanState->setVal(0); // inactive
            else
                currentFanState->setVal(1); // idle
        }
    }
}

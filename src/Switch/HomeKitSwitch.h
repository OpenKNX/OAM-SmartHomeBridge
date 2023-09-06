#pragma once
#include "HomeSpan.h"
#include "KnxChannelSwitch.h"

class HomeKitSwitch : public SwitchBridge
{
    int device;
    Characteristic::On *power;
    class ServiceImplementation : Service::LightBulb
    {
        HomeKitSwitch* parent;
        bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementation(HomeKitSwitch* parent) : parent(parent)
        {
        }
    };
public:
    HomeKitSwitch(int device);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setPower(bool value) override;
};
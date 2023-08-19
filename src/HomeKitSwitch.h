#pragma once
#include "HomeSpan.h"
#include "KnxChannelSwitch.h"

class HomeKitSwitch : public ISwitchBridge
{
    int device;
    KnxChannelSwitch *switchDevice;
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
    virtual void initialize(KnxChannelSwitch *switchDevice) override;

    boolean update();
    virtual void setPower(bool value) override;
};
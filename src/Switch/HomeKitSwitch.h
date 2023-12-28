#pragma once
#include "HomeSpan.h"
#include "KnxChannelSwitch.h"

class HomeKitSwitch : public SwitchBridge
{
    int device;
    Characteristic::On *power;
    class ServiceImplementationLightBulb : public Service::LightBulb
    {
        HomeKitSwitch* parent;
        bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementationLightBulb(HomeKitSwitch* parent) : parent(parent)
        {
        }
    };
    class ServiceImplementationSwitch : public Service::Switch
    {
        HomeKitSwitch* parent;
        bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementationSwitch(HomeKitSwitch* parent) : parent(parent)
        {
        }
    };
    class ServiceImplementationOutlet : public Service::Outlet
    {
        HomeKitSwitch* parent;
        bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementationOutlet(HomeKitSwitch* parent) : parent(parent)
        {
        }
    };
public:
    HomeKitSwitch(int device);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setPower(bool value) override;
};
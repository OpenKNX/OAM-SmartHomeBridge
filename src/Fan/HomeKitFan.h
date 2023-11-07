#pragma once
#include "HomeSpan.h"
#include "KnxChannelFan.h"

class HomeKitFan : public FanBridge
{
    int device;
    Characteristic::CurrentFanState *currentFanState = nullptr;
    Characteristic::TargetFanState *targetFanState = nullptr;
    Characteristic::Active *active;
    bool automatic = false;
    class ServiceImplementation : Service::Fan
    {
        HomeKitFan* parent;
        bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementation(HomeKitFan* parent) : parent(parent)
        {
        }
    };
public:
    HomeKitFan(int device);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setPower(bool power) override;
    virtual void setAutomatic(bool automatic) override;
};
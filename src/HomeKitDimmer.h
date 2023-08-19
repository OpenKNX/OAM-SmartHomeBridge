#pragma once
#include "HomeSpan.h"
#include "KnxChannelDimmer.h"

class HomeKitDimmer : public IDimmerBridge
{
    int device;
    KnxChannelDimmer *dimmerDevice;
    Characteristic::On *power;
    Characteristic::Brightness *level;
    class ServiceImplementation : Service::LightBulb
    {
        HomeKitDimmer* parent;
        virtual bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementation(HomeKitDimmer* parent) : parent(parent)
        {
        }
    };
public:
    HomeKitDimmer(int device);
    virtual void initialize(KnxChannelDimmer *dimmerDevice) override;

    boolean update();
    virtual void setBrightness(uint8_t brightness) override;
};
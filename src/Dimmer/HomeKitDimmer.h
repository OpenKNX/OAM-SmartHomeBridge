#pragma once
#include "HomeSpan.h"
#include "KnxChannelDimmer.h"

class HomeKitDimmer : public DimmerBridge
{
    int device;
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
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setBrightness(uint8_t brightness) override;
};
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
        bool update()
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
    void initialize(KnxChannelDimmer *dimmerDevice);

    boolean update();
    virtual void setBrightness(uint8_t brightness);
};
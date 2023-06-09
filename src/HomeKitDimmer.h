#pragma once
#include "HomeSpan.h"
#include "KnxChannelDimmer.h"

class HomeKitDimmer : public IDimmerInterface
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
    virtual bool getPower();
    virtual void setPower(bool value);
    virtual int getBrightness();
    virtual void setBrightness(int brightness);
};
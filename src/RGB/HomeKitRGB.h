#pragma once
#include "HomeSpan.h"
#include "KnxChannelRGB.h"

class HomeKitRGB : public RGBBridge
{
    int device;
    Characteristic::On *power;
    Characteristic::Brightness *brighness;
    Characteristic::Saturation *saturation;
    Characteristic::Hue *hue;
    class ServiceImplementation : public Service::LightBulb
    {
        HomeKitRGB* parent;
        virtual bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementation(HomeKitRGB* parent) : parent(parent)
        {
        }
    };
public:
    HomeKitRGB(int device);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setRGB(uint32_t rgb) override;
    virtual void setPower(bool on) override;
};
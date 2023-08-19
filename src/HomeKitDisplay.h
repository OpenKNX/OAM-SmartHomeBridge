#pragma once
#include "HomeSpan.h"
#include "KnxChannelDisplay.h"

class HomeKitDisplay : public IDisplayBridge
{
    int device;
    KnxChannelDisplay *displayDevice;
    SpanCharacteristic *currentValue = NULL;
public:
    HomeKitDisplay(int device);
    virtual void initialize(KnxChannelDisplay *displayDevice) override;
    void createAccessory();

    boolean update();
    virtual void setValue(double value) override;
};
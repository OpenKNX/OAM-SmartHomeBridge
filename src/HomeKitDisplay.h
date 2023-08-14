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
    void initialize(KnxChannelDisplay *displayDevice);
    void createAccessory();

    boolean update();
    virtual void setValue(double value);
};
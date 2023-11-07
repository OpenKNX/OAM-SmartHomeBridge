#pragma once
#include "HomeSpan.h"
#include "KnxChannelDisplay.h"

class HomeKitDisplay : public DisplayBridge
{
    int device;
    SpanCharacteristic *currentValue = nullptr;
public:
    HomeKitDisplay(int device);
    virtual void setup(uint8_t _channelIndex) override;
    void createAccessory();

    boolean update();
    virtual void setValue(double value) override;
};
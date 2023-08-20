#pragma once
#include "HomeSpan.h"
#include "KnxChannelDisplay.h"

class HomeKitDisplay : public DisplayBridge
{
    int device;
    SpanCharacteristic *currentValue = NULL;
public:
    HomeKitDisplay(int device);
    virtual void setup() override;
    void createAccessory();

    boolean update();
    virtual void setValue(double value) override;
};
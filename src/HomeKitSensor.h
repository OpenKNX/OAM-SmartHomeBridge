#pragma once
#include "HomeSpan.h"
#include "KnxChannelSensor.h"

class HomeKitSensor : public SensorBridge
{
    int device;
    SpanCharacteristic *currentValue = NULL;
public:
    HomeKitSensor(int device);
    virtual void setup() override;
    void createAccessory();

    boolean update();
    virtual void setDetected(bool value) override;
};
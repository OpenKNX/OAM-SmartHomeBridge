#pragma once
#include "HomeSpan.h"
#include "KnxChannelSensor.h"

class HomeKitSensor : public ISensorBridge
{
    int device;
    KnxChannelSensor *sensorDevice;
    SpanCharacteristic *currentValue = NULL;
public:
    HomeKitSensor(int device);
    virtual void initialize(KnxChannelSensor *sensorDevice) override;
    void createAccessory();

    boolean update();
    virtual void setDetected(bool value) override;
};
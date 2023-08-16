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
    void initialize(KnxChannelSensor *sensorDevice);
    void createAccessory();

    boolean update();
    virtual void setDetected(bool value);
};
#pragma once
#include "KnxChannelBase.h"

class KnxChannelSensor;

class SensorBridge : public ChannelBridgeBase<KnxChannelSensor>
{
public:
    virtual void setDetected(bool detected) = 0;
};

enum SensorType
{
    SensorTypeContact = 70,
    SensorTypeMotion = 71,
    SensorTypeOccupancy = 72,
    SensorTypeLeak = 73,
    SensorTypeSmoke = 74,
    SensorTypeCarbonDioxid = 75,
    SensorTypeCarbonMonoxid = 76
};
             
class KnxChannelSensor : public KnxChannelBase
{
    public:
        DynamicPointerArray<SensorBridge > *sensorBridges;
        KnxChannelSensor(DynamicPointerArray<SensorBridge > *sensorBridges, uint16_t channelIndex);
        SensorType getSensorType();
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual const std::string name() override;
};

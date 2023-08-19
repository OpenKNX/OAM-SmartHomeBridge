#pragma once
#include "KnxChannelBase.h"

class KnxChannelSensor;

class ISensorBridge
{
    public:
    virtual void initialize(KnxChannelSensor* sensorDevice) = 0;
    virtual void setDetected(bool detected) = 0;
};

enum SensorType
{
    SensorTypeContact,
    SensorTypeMotion,
    SensorTypeOccupancy,
    SensorTypeLeak,
    SensorTypeSmoke,
    SensorTypeCarbonDioxid,
    SensorTypeCarbonMonoxid
};
             
class KnxChannelSensor : public KnxChannelBase
{
    public:
        std::list<ISensorBridge *> *sensorBridges;
        KnxChannelSensor(std::list<ISensorBridge *> *sensorBridges, uint16_t channelIndex);
        SensorType getSensorType();
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;
};

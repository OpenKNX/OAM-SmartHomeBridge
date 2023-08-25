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
        std::list<SensorBridge *> *sensorBridges;
        KnxChannelSensor(std::list<SensorBridge *> *sensorBridges, uint16_t channelIndex);
        SensorType getSensorType();
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual const std::string name() override;
};

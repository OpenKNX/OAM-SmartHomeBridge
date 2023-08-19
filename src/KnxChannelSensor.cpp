#include <knx.h>
#include "Bridge.h"
#include "KnxChannelSensor.h"

#define KO_SENSOR_FEEDBACK   KoBRI_KO1_, DPT_Switch

KnxChannelSensor::KnxChannelSensor(std::list<ISensorBridge *> *sensorBridges, uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      sensorBridges(sensorBridges)
{
    for (std::list<ISensorBridge *>::iterator it = sensorBridges->begin(); it != sensorBridges->end(); ++it)
         (*it)->initialize(this);
}

SensorType KnxChannelSensor::getSensorType()
{
    return (SensorType) ParamBRI_CHContactAlarmSensorType;
}

void KnxChannelSensor::setup()
{
    goSetWithoutSend(KO_SENSOR_FEEDBACK, ParamBRI_CHContactAlarmSensorInvert == 1);
    goSendReadRequest(KO_SENSOR_FEEDBACK);
}

void KnxChannelSensor::processInputKo(GroupObject &ko)
{
    if (isGo(ko, KoBRI_KO1_))
    {
        bool value = goGet(KO_SENSOR_FEEDBACK);
        if (ParamBRI_CHContactAlarmSensorInvert)
            value = !value;
        for (std::list<ISensorBridge *>::iterator it = sensorBridges->begin(); it != sensorBridges->end(); ++it)
        {
            (*it)->setDetected(value);
        }
    }
}

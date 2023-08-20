#include "HomeKitSensor.h"

HomeKitSensor::HomeKitSensor(int device) :
    device(device)
{
}

void HomeKitSensor::createAccessory()
{
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
}

void HomeKitSensor::setup()
{
    switch (_channel->getSensorType())
    {
        case SensorType::SensorTypeContact:
            createAccessory();
            new Service::ContactSensor();
            currentValue = new Characteristic::ContactSensorState(0);
            break;
        case SensorType::SensorTypeMotion:
            createAccessory();
            new Service::MotionSensor();
            currentValue = new Characteristic::MotionDetected();
            break;
        case SensorType::SensorTypeOccupancy:
            createAccessory();
            new Service::OccupancySensor();
            currentValue = new Characteristic::OccupancyDetected();
            break;
        case SensorType::SensorTypeLeak:
            createAccessory();
            new Service::LeakSensor();
            currentValue = new Characteristic::LeakDetected();
            break;
        case SensorType::SensorTypeSmoke:
            createAccessory();
            new Service::SmokeSensor();
            currentValue = new Characteristic::SmokeDetected();
            break;
        case SensorType::SensorTypeCarbonDioxid:
            createAccessory();
            new Service::CarbonDioxideSensor();
            currentValue = new Characteristic::CarbonDioxideDetected();
            break;
        case SensorType::SensorTypeCarbonMonoxid:
            createAccessory();
            new Service::CarbonMonoxideSensor();
            currentValue = new Characteristic::CarbonMonoxideDetected();
            break;
        default:
            Serial.print("Unkown Sensor Type ");
            Serial.print(_channel->getSensorType());
        break;
    }
}

void HomeKitSensor::setDetected(bool value)
{
    if (currentValue != NULL)
        currentValue->setVal(value ? 1 : 0);
}

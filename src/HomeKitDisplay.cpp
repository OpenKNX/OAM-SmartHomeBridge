#include "HomeKitDisplay.h"

HomeKitDisplay::HomeKitDisplay(int device) :
    device(device)
{
}

void HomeKitDisplay::createAccessory()
{
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
}

void HomeKitDisplay::setup()
{
    switch (_channel->getDisplayType())
    {
        case DisplayType::DisplayTypeTemperature:
            createAccessory();
            new Service::TemperatureSensor();
            currentValue = new Characteristic::CurrentTemperature();
            break;
        case DisplayType::DisplayTypeHumidity:
            createAccessory();
            new Service::HumiditySensor();
            currentValue = new Characteristic::CurrentRelativeHumidity();
            break;
        case DisplayType::DisplayTypeLux:
            createAccessory();
            new Service::LightSensor();
            currentValue = new Characteristic::CurrentAmbientLightLevel();
            break;
        default:
            Serial.print("Unkown Display Type ");
            Serial.print(_channel->getDisplayType());
        break;
    }
}

void HomeKitDisplay::setValue(double value)
{
    if (currentValue != nullptr)
        currentValue->setVal(value);
}

#include "HomeKitThermostat.h"

HomeKitThermostat::HomeKitThermostat(int device) :
    device(device)
{
}

void HomeKitThermostat::initialize(KnxChannelThermostat *thermostatDevice)
{
    this->thermostatDevice = thermostatDevice;
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(thermostatDevice->deviceName);
    new ServiceImplementation(this);
        currentHeaterCoolerState = new Characteristic::CurrentHeatingCoolingState();
        targetHeaterCoolerState = new Characteristic::TargetHeatingCoolingState();
        currentTemperature = new Characteristic::CurrentTemperature();
        targetTemperature = new Characteristic::TargetTemperature(KnxChannelThermostat::DEFAULT_TEMPERATURE);
        temperatureDisplayUnits = new Characteristic::TemperatureDisplayUnits();
    ThermostatDisplayUnit unit = thermostatDevice->GetDisplayTemperaturUnit();
    switch (unit)
    {
        case ThermostatDisplayUnit::ThermostatDisplayUnitCelsius:
            temperatureDisplayUnits->setVal(0); // Celsius;
            break;
        case ThermostatDisplayUnit::ThermostatDisplayUnitFahrenheit:
            temperatureDisplayUnits->setVal(1); // Fahrenheit;
            break;
    }
}

boolean HomeKitThermostat::update()
{
    if (targetTemperature->updated())
        thermostatDevice->commandTargetTemperature(this, targetTemperature->getNewVal());

    if (targetHeaterCoolerState->updated())
    {
        switch(targetHeaterCoolerState->getNewVal())
        {
            case 0: // off
                Serial.println("OFF");
                return thermostatDevice->commandMode(this, ThermostatMode::ThermostatModeDisabled);
            case 1: // heat
                Serial.println("Heat");
                return thermostatDevice->commandMode(this, ThermostatMode::ThermostatModeHeating);
            case 2: // cool
                Serial.println("Cool");
                return thermostatDevice->commandMode(this, ThermostatMode::ThermostatModeCooling);     
            case 3: // auto
                Serial.println("Auto");
                return thermostatDevice->commandMode(this, ThermostatMode::ThermostatModeAutoHeatingCooling);
        }
    }
    return true;
}

void HomeKitThermostat::setCurrentTemperature(float targetThemerature)
{
    currentTemperature->setVal(targetThemerature);
}

void HomeKitThermostat::setMode(ThermostatMode mode)
{
    switch (mode)
    {
        case ThermostatMode::ThermostatModeDisabled:
            currentHeaterCoolerState->setVal(0); // off
            break;
        case ThermostatMode::ThermostatModeHeating:
            currentHeaterCoolerState->setVal(1); // heat
            break;
        case ThermostatMode::ThermostatModeCooling:
            currentHeaterCoolerState->setVal(2); // cool
            break;
        case ThermostatMode::ThermostatModeAutoHeatingCooling:
            currentHeaterCoolerState->setVal(3); // auto
            break;
    }
}
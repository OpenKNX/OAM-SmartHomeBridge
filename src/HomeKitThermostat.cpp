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
        new Characteristic::Name(thermostatDevice->getNameInUTF8());
    new ServiceImplementation(this);
        currentHeaterCoolerState = new Characteristic::CurrentHeatingCoolingState();
        targetHeaterCoolerState = new Characteristic::TargetHeatingCoolingState();
        currentTemperature = new Characteristic::CurrentTemperature();
        targetTemperature = new Characteristic::TargetTemperature(KnxChannelThermostat::DEFAULT_TEMPERATURE);
        targetTemperature->setRange(10.0, 38.0, 0.5);
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
    {
        Serial.print("Homekit sends ");
        Serial.print(targetTemperature->getNewVal<double>());
        thermostatDevice->commandTargetTemperature(this, targetTemperature->getNewVal<double>());
    }
    if (targetHeaterCoolerState->updated())
    {
        switch(targetHeaterCoolerState->getNewVal())
        {
            case 0: // off
                Serial.println("OFF");
                return thermostatDevice->commandMode(this, ThermostatMode::ThermostatModeOff);
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

void HomeKitThermostat::setTargetTemperature(double temperature)
{
    Serial.print("Set Target Temperature");
    Serial.println(temperature);
    targetTemperature->setVal(temperature);
}

void HomeKitThermostat::setCurrentTemperature(double temperature)
{
    currentTemperature->setVal(temperature);
}

void HomeKitThermostat::setMode(ThermostatMode mode)
{
    switch (mode)
    {
        case ThermostatMode::ThermostatModeOff:
            targetHeaterCoolerState->setVal(0); // Off
            break;
        case ThermostatMode::ThermostatModeHeating:
            targetHeaterCoolerState->setVal(1); // Heating
            break;
        case ThermostatMode::ThermostatModeCooling:
            targetHeaterCoolerState->setVal(2); // Cooling
            break;
        case ThermostatMode::ThermostatModeAutoHeatingCooling:
            targetHeaterCoolerState->setVal(3); // Auto
            break;
    }
}

void HomeKitThermostat::setCurrentState(ThermostatCurrentState state)
{
    switch (state)
    {
        case ThermostatCurrentState::ThermostatCurrentStateOff:
            currentHeaterCoolerState->setVal(0); // off
            break;
        case ThermostatCurrentState::ThermostatCurrentStateHeating:
            currentHeaterCoolerState->setVal(1); // heat
            break;
        case ThermostatCurrentState::ThermostatCurrentStateCooling:
            currentHeaterCoolerState->setVal(2); // cool
            break;
    }
}

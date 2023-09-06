#pragma once
#include "HomeSpan.h"
#include "KnxChannelThermostat.h"

class HomeKitThermostat : public ThermostatBridge
{
    int device;
    Characteristic::CurrentHeatingCoolingState *currentHeaterCoolerState;
    Characteristic::TargetHeatingCoolingState *targetHeaterCoolerState;
    Characteristic::CurrentTemperature *currentTemperature;
    Characteristic::TargetTemperature *targetTemperature;
    Characteristic::TemperatureDisplayUnits *temperatureDisplayUnits;
    class ServiceImplementation : Service::Thermostat
    {
        HomeKitThermostat* parent;
        bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementation(HomeKitThermostat* parent) : parent(parent)
        {
        }
    };
public:
    HomeKitThermostat(int device);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setTargetTemperature(double temperature) override;
    virtual void setCurrentTemperature(double temperature) override;
    virtual void setMode(ThermostatMode mode) override;
    virtual void setCurrentState(ThermostatCurrentState state) override;
};
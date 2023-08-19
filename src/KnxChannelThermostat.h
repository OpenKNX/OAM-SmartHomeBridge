#pragma once
#include "KnxChannelBase.h"

class KnxChannelThermostat;

enum ThermostatMode
{
    ThermostatModeOff,
    ThermostatModeHeating,
    ThermostatModeCooling,
    ThermostatModeAutoHeatingCooling,
};

enum ThermostatCurrentState
{
    ThermostatCurrentStateOff,
    ThermostatCurrentStateHeating,
    ThermostatCurrentStateCooling,
};

enum ThermostatDisplayUnit
{
    ThermostatDisplayUnitCelsius,
    ThermostatDisplayUnitFahrenheit,
};

class IThermostatBridge
{
    public:
    virtual void initialize(KnxChannelThermostat* thermostatDevice) = 0;
    virtual void setTargetTemperature(double temperature) = 0;
    virtual void setCurrentTemperature(double temperature) = 0;
    virtual void setMode(ThermostatMode mode) = 0;
    virtual void setCurrentState(ThermostatCurrentState currentState) = 0;
};

class KnxChannelThermostat : public KnxChannelBase
{
    public:
        static float DEFAULT_TEMPERATURE;
        std::list<IThermostatBridge *> *thermostatBridges;
        KnxChannelThermostat(std::list<IThermostatBridge *> *thermostatBridges, uint16_t channelIndex);
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandTargetTemperature(IThermostatBridge* thermostatBridge, double temperature);
        bool commandMode(IThermostatBridge* thermostatBridge, ThermostatMode mode);
        ThermostatDisplayUnit GetDisplayTemperaturUnit();
};

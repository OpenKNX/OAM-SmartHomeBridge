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

class ThermostatBridge : public ChannelBridgeBase<KnxChannelThermostat>
{
public:
    virtual void setTargetTemperature(double temperature) = 0;
    virtual void setCurrentTemperature(double temperature) = 0;
    virtual void setMode(ThermostatMode mode) = 0;
    virtual void setCurrentState(ThermostatCurrentState currentState) = 0;
};

class KnxChannelThermostat : public KnxChannelBase
{
    public:
        static float DEFAULT_TEMPERATURE;
        DynamicPointerArray<ThermostatBridge> *thermostatBridges;
        KnxChannelThermostat(DynamicPointerArray<ThermostatBridge > *thermostatBridges, uint16_t channelIndex);
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandTargetTemperature(ThermostatBridge* thermostatBridge, double temperature);
        bool commandMode(ThermostatBridge* thermostatBridge, ThermostatMode mode);
        ThermostatDisplayUnit GetDisplayTemperaturUnit();
        virtual const std::string name() override;
};

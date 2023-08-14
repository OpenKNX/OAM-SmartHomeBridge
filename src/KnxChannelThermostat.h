#pragma once
#include "KnxChannelBase.h"

class KnxChannelThermostat;

enum ThermostatMode
{
    ThermostatModeDisabled,
    ThermostatModeHeating,
    ThermostatModeCooling,
    ThermostatModeAutoHeatingCooling,
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
    virtual void setCurrentTemperature(float targetThemerature) = 0;
    virtual void setMode(ThermostatMode mode) = 0;
};

class KnxChannelThermostat : public KnxChannelBase
{
    public:
        static float DEFAULT_TEMPERATURE;
        std::list<IThermostatBridge *> *thermostatBridges;
        KnxChannelThermostat(std::list<IThermostatBridge *> *thermostatBridges, uint16_t channelIndex);
    protected:
        virtual void loop(unsigned long now, bool initalize);
        virtual void received(GroupObject& groupObject);

    public:
        void commandTargetTemperature(IThermostatBridge* thermostatBridge, float targetTemperature);
        bool commandMode(IThermostatBridge* thermostatBridge, ThermostatMode mode);
        ThermostatDisplayUnit GetDisplayTemperaturUnit();
};

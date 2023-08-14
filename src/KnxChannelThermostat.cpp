#include <knx.h>
#include "Bridge.h"
#include "KnxChannelThermostat.h"

#define KO_TARGET_TEMPERATURE           KoBRI_KO1_, DPT_Value_Temp
#define KO_CURRENT_TEMPERATUR_FEEDBACK  KoBRI_KO2_, DPT_Value_Temp
#define KO_HEADING                      KoBRI_KO3_, DPT_Switch
#define KO_HEADING_FEEDBACK             KoBRI_KO4_, DPT_Switch
#define KO_COOLING                      KoBRI_KO5_, DPT_Switch
#define KO_COOLING_FEEDBACK             KoBRI_KO6_, DPT_Switch

float KnxChannelThermostat::DEFAULT_TEMPERATURE = 22;

KnxChannelThermostat::KnxChannelThermostat(std::list<IThermostatBridge *> *thermostatBridges, uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      thermostatBridges(thermostatBridges)
{
    for (std::list<IThermostatBridge *>::iterator it = thermostatBridges->begin(); it != thermostatBridges->end(); ++it)
         (*it)->initialize(this);
}

ThermostatDisplayUnit KnxChannelThermostat::GetDisplayTemperaturUnit()
{
    return (ThermostatDisplayUnit) ParamBRI_CHThermostatTemperaturUnitType;
}

void KnxChannelThermostat::commandTargetTemperature(IThermostatBridge* thermostatBridge, float targetTemperature)
{
    Serial.print(componentName);
    Serial.println(" device receive changed");
    Serial.print("Target Temperatur: ");
    Serial.println(targetTemperature);
    for (std::list<IThermostatBridge *>::iterator it = thermostatBridges->begin(); it != thermostatBridges->end(); ++it)
    {
        if ((*it) != thermostatBridge)
        {
            (*it)->setCurrentTemperature(targetTemperature);
        }
    }
    goSet(KO_TARGET_TEMPERATURE, targetTemperature, true);
}


bool KnxChannelThermostat::commandMode(IThermostatBridge* thermostatBridge, ThermostatMode mode)
{
    Serial.print(componentName);
    Serial.println(" device receive changed");
    Serial.print("Mode: ");
    Serial.println(mode);
    for (std::list<IThermostatBridge *>::iterator it = thermostatBridges->begin(); it != thermostatBridges->end(); ++it)
    {
        if ((*it) != thermostatBridge)
        {
            (*it)->setMode(mode);
        }
    }
    switch (mode)
    {
        case ThermostatMode::ThermostatModeDisabled:
            goSet(KO_HEADING, false, true);
            goSet(KO_COOLING, false, true);
            break;
        case ThermostatMode::ThermostatModeHeating:
            if (ParamBRI_CHThermostatMode & 1 == 0)
                return false; 
            goSet(KO_HEADING, true, true);
            goSet(KO_COOLING, false, true);
            break;
        case ThermostatMode::ThermostatModeCooling:
            if (ParamBRI_CHThermostatMode & 2 == 0)
                return false; 
            goSet(KO_HEADING, false, true);
            goSet(KO_COOLING, true, true);
            break;
        case ThermostatMode::ThermostatModeAutoHeatingCooling:
            if (ParamBRI_CHThermostatMode & 3 == 0)
                return false; 
            goSet(KO_HEADING, true, true);
            goSet(KO_COOLING, true, true);
            break;
    }
    return true;
}

void KnxChannelThermostat::loop(unsigned long now, bool initalize)
{
    if (initalize)
    {
        goSetWithoutSend(KO_CURRENT_TEMPERATUR_FEEDBACK, DEFAULT_TEMPERATURE);
        goSendReadRequest(KO_CURRENT_TEMPERATUR_FEEDBACK);  
        goSetWithoutSend(KO_HEADING_FEEDBACK, false);
        goSendReadRequest(KO_HEADING_FEEDBACK);
        goSetWithoutSend(KO_COOLING_FEEDBACK, false);
        goSendReadRequest(KO_COOLING_FEEDBACK);
    }
}

void KnxChannelThermostat::received(GroupObject &groupObject)
{
    if (isGo(groupObject, KO_CURRENT_TEMPERATUR_FEEDBACK))
    {
        float temperature = goGet(KO_CURRENT_TEMPERATUR_FEEDBACK);
        for (std::list<IThermostatBridge *>::iterator it = thermostatBridges->begin(); it != thermostatBridges->end(); ++it)
        {
            (*it)->setCurrentTemperature(temperature);
        }
    }
    else if (isGo(groupObject, KO_COOLING_FEEDBACK) || isGo(groupObject, KO_HEADING_FEEDBACK))
    {
        bool cooling = goGet(KO_COOLING_FEEDBACK);
        bool heading = goGet(KO_HEADING_FEEDBACK);
        ThermostatMode mode = ThermostatMode::ThermostatModeDisabled;
        if (cooling)
            mode = ThermostatMode::ThermostatModeCooling;
        if (heading)
            mode = ThermostatMode::ThermostatModeHeating;

        for (std::list<IThermostatBridge *>::iterator it = thermostatBridges->begin(); it != thermostatBridges->end(); ++it)
        {
            (*it)->setMode(mode);
        }
    }
}

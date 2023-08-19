#include <knx.h>
#include "Bridge.h"
#include "KnxChannelThermostat.h"

#define KO_TARGET_TEMPERATURE              KoBRI_KO1_, DPT_Value_Temp
#define KO_TARGET_TEMPERATURE_FEEDBACK     KoBRI_KO2_, DPT_Value_Temp
#define KO_CURRENT_TEMPERATUR_FEEDBACK     KoBRI_KO3_, DPT_Value_Temp
#define KO_HEADING                         KoBRI_KO4_, DPT_Switch
#define KO_HEADING_FEEDBACK                KoBRI_KO5_, DPT_Switch
#define KO_HEADING_ACTIVE_FEEDBACK         KoBRI_KO6_, DPT_Switch
#define KO_HEADING_ACTIVE_PERCENT_FEEDBACK KoBRI_KO6_, DPT_Scaling
#define KO_COOLING                         KoBRI_KO7_, DPT_Switch
#define KO_COOLING_FEEDBACK                KoBRI_KO8_, DPT_Switch
#define KO_COOLING_ACTIVE_FEEDBACK         KoBRI_KO9_, DPT_Switch
#define KO_COOLING_ACTIVE_PERCENT_FEEDBACK KoBRI_KO9_, DPT_Scaling


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

void KnxChannelThermostat::commandTargetTemperature(IThermostatBridge* thermostatBridge, double temperature)
{
    Serial.print(getName());
    Serial.println(" device receive changed");
    Serial.print("Target Temperatur: ");
    Serial.println(temperature);
    for (std::list<IThermostatBridge *>::iterator it = thermostatBridges->begin(); it != thermostatBridges->end(); ++it)
    {
        if ((*it) != thermostatBridge)
        {
            (*it)->setCurrentTemperature(temperature);
        }
    }
    goSet(KO_TARGET_TEMPERATURE, temperature, true);
}


bool KnxChannelThermostat::commandMode(IThermostatBridge* thermostatBridge, ThermostatMode mode)
{
    Serial.print(getName());
    Serial.println(" device receive changed");
    Serial.print("Mode: ");
    Serial.println(mode);
    bool heading = false;
    bool cooling = false;
    switch (mode)
    {
        case ThermostatMode::ThermostatModeOff:
            heading = false;
            cooling = false;
            break;
        case ThermostatMode::ThermostatModeHeating:
            if (ParamBRI_CHThermostatMode & 1 == 0)
                return false; 
            heading = true;
            cooling = false;
            break;
        case ThermostatMode::ThermostatModeCooling:
            if (ParamBRI_CHThermostatMode & 2 == 0)
                return false; 
            heading = false;
            cooling = true;
            break;
        case ThermostatMode::ThermostatModeAutoHeatingCooling:
            if (ParamBRI_CHThermostatMode & 3 == 0)
                return false; 
            heading = true;
            cooling = true;
            break;
    }

    // Handle Switch or lock mode to set KO
    if (ParamBRI_CHThermostatKoModeHeating == 0)
        goSet(KO_HEADING, heading, true);
    else
        goSet(KO_HEADING, !heading, true);

    if (ParamBRI_CHThermostatKoModeHeating == 0)
        goSet(KO_COOLING, heading, true);
    else
        goSet(KO_COOLING, !heading, true);
    
    // Inform other bridges
    for (std::list<IThermostatBridge *>::iterator it = thermostatBridges->begin(); it != thermostatBridges->end(); ++it)
    {
        if ((*it) != thermostatBridge)
        {
            (*it)->setMode(mode);
        }
    }
    return true;
}

void KnxChannelThermostat::setup()
{
    goSetWithoutSend(KO_CURRENT_TEMPERATUR_FEEDBACK, DEFAULT_TEMPERATURE);
    goSendReadRequest(KO_CURRENT_TEMPERATUR_FEEDBACK);  
    goSetWithoutSend(KO_HEADING_FEEDBACK, ParamBRI_CHThermostatKoModeHeatingFeedback == 1);
    goSendReadRequest(KO_HEADING_FEEDBACK);
    if (ParamBRI_CHThemostateHeatingFeedbackKoType == 0)
    {
        goSetWithoutSend(KO_HEADING_ACTIVE_FEEDBACK, false);
        goSendReadRequest(KO_HEADING_ACTIVE_FEEDBACK);
    }
    else
    {
        goSetWithoutSend(KO_HEADING_ACTIVE_PERCENT_FEEDBACK, false);
        goSendReadRequest(KO_HEADING_ACTIVE_PERCENT_FEEDBACK);
    }
    goSetWithoutSend(KO_COOLING_FEEDBACK, ParamBRI_CHThermostatKoModeCoolingFeedback == 1);
    goSendReadRequest(KO_COOLING_FEEDBACK);
    if (ParamBRI_CHThemostateCoolingFeedbackKoType == 0)
    {
        goSetWithoutSend(KO_COOLING_ACTIVE_FEEDBACK, false);
        goSendReadRequest(KO_COOLING_ACTIVE_FEEDBACK);
    }
    else
    {
        goSetWithoutSend(KO_COOLING_ACTIVE_PERCENT_FEEDBACK, false);
        goSendReadRequest(KO_COOLING_ACTIVE_PERCENT_FEEDBACK);
    }
}

void KnxChannelThermostat::processInputKo(GroupObject &ko)
{
    if (isGo(ko, KO_TARGET_TEMPERATURE_FEEDBACK))
    {
        double temperature = goGet(KO_TARGET_TEMPERATURE_FEEDBACK);
        Serial.print("Target temperature received ");
        Serial.println(temperature);
        for (std::list<IThermostatBridge *>::iterator it = thermostatBridges->begin(); it != thermostatBridges->end(); ++it)
        {
            (*it)->setTargetTemperature(temperature);
        }
    }
    else if (isGo(ko, KO_CURRENT_TEMPERATUR_FEEDBACK))
    {
        double temperature = goGet(KO_CURRENT_TEMPERATUR_FEEDBACK);
        Serial.print("Current temperature received ");
        Serial.println(temperature);
        for (std::list<IThermostatBridge *>::iterator it = thermostatBridges->begin(); it != thermostatBridges->end(); ++it)
        {
            (*it)->setCurrentTemperature(temperature);
        }
    }
    else if (isGo(KoBRI_KO1_, KO_COOLING_FEEDBACK) || isGo(ko, KO_HEADING_FEEDBACK))
    {
        bool heading = goGet(KO_HEADING_FEEDBACK);
        if ((bool)ParamBRI_CHThermostatKoModeHeatingFeedback)
            heading = !heading;
        bool cooling = goGet(KO_COOLING_FEEDBACK);
        if ((bool)ParamBRI_CHThermostatKoModeCoolingFeedback)
            cooling = !cooling;

        ThermostatMode mode = ThermostatMode::ThermostatModeOff;
        if (cooling && heading)
            mode = ThermostatMode::ThermostatModeAutoHeatingCooling;
        else if (cooling)
            mode = ThermostatMode::ThermostatModeCooling;
        else if (heading)
            mode = ThermostatMode::ThermostatModeHeating;
 
        for (std::list<IThermostatBridge *>::iterator it = thermostatBridges->begin(); it != thermostatBridges->end(); ++it)
        {
            (*it)->setMode(mode);
        }
    }
    else if (isGo(ko, KO_HEADING_ACTIVE_FEEDBACK) || isGo(ko, KO_COOLING_ACTIVE_FEEDBACK))
    {
        bool heading = ParamBRI_CHThemostateHeatingFeedbackKoType == 0 ? (boolean) goGet(KO_HEADING_ACTIVE_FEEDBACK) : 0 < (int)goGet(KO_HEADING_ACTIVE_PERCENT_FEEDBACK);
        if (heading && isGo(ko, KO_HEADING_ACTIVE_FEEDBACK))
            goSetWithoutSend(KO_COOLING_FEEDBACK, false);
        bool cooling = ParamBRI_CHThemostateCoolingFeedbackKoType == 0 ? (boolean) goGet(KO_COOLING_ACTIVE_FEEDBACK) : 0 < (int)goGet(KO_COOLING_ACTIVE_PERCENT_FEEDBACK);
        if (cooling && isGo(ko, KO_COOLING_ACTIVE_FEEDBACK))
            goSetWithoutSend(KO_HEADING_FEEDBACK, false);
        
        ThermostatCurrentState state = ThermostatCurrentState::ThermostatCurrentStateOff;
        if (heading)
            state = ThermostatCurrentState::ThermostatCurrentStateHeating;
        if (cooling)
            state = ThermostatCurrentState::ThermostatCurrentStateCooling;

        for (std::list<IThermostatBridge *>::iterator it = thermostatBridges->begin(); it != thermostatBridges->end(); ++it)
        {
            (*it)->setCurrentState(state);
        }
    }
}

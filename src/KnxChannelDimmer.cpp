#include <knx.h>
#include "Bridge.h"
#include "KnxChannelDimmer.h"

#define KO_DIMMER           KoBRI_KO1_, DPT_Scaling
#define KO_DIMMER_FEEDBACK  KoBRI_KO2_, DPT_Scaling


enum DimmerSwitchBehavior
{
    LastBrightness = 201,
    LastBrightnessLessThan100 = 200
};


KnxChannelDimmer::KnxChannelDimmer(std::list<IDimmerBridge *> *dimmerBridges, uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      dimmerBridges(dimmerBridges)
{
    for (std::list<IDimmerBridge *>::iterator it = dimmerBridges->begin(); it != dimmerBridges->end(); ++it)
         (*it)->initialize(this);
}

void KnxChannelDimmer::commandBrightness(IDimmerBridge* dimmerBridge, uint8_t brightness)
{
    Serial.print(getName());
    Serial.println(" device receive changed");
    Serial.print("Brightness: ");
    Serial.println(brightness);
    if (brightness > 0)
        lastBrighness = brightness;
    uint8_t knxValue = brightness;
    for (std::list<IDimmerBridge *>::iterator it = dimmerBridges->begin(); it != dimmerBridges->end(); ++it)
    {
        if ((*it) != dimmerBridge)
        {
            (*it)->setBrightness(brightness);
        }
    }
    goSetWithoutSend(KO_DIMMER_FEEDBACK, knxValue);
    goSet(KO_DIMMER, knxValue, true);
}

void KnxChannelDimmer::commandPower(IDimmerBridge* dimmerBridge, bool power)
{
    if (power)
    {
        uint8_t configValue = 0 == (uint8_t) goGet(KO_DIMMER_FEEDBACK) 
            ? ParamBRI_CHDimmerSwitchOnBehavior 
            : ParamBRI_CHDimmerSwitchOn2Behavior;
        switch((DimmerSwitchBehavior) configValue)
        {
            case DimmerSwitchBehavior::LastBrightness:
                commandBrightness(NULL, lastBrighness);
                break;
            case DimmerSwitchBehavior::LastBrightnessLessThan100:
                commandBrightness(NULL, lastBrighnessLessThan100);
                break;
            default:
                commandBrightness(NULL, configValue); // Direct percentage value
        }
    }
    else
    {
        commandBrightness(NULL, 0);
    }
}

void KnxChannelDimmer::loop(unsigned long now, bool initalize)
{
    if (initalize)
    {
        goSetWithoutSend(KO_DIMMER, 0);
        goSetWithoutSend(KO_DIMMER_FEEDBACK, 0);
        goSendReadRequest(KO_DIMMER_FEEDBACK);
    }
}

void KnxChannelDimmer::received(GroupObject &groupObject)
{
    if (isGo(groupObject, KO_DIMMER_FEEDBACK))
    {
        uint8_t brightness = goGet(KO_DIMMER_FEEDBACK);
        if (brightness > 0)
        {
            lastBrighness = brightness;
            if (brightness < 100)
                lastBrighnessLessThan100 = brightness;
        }    
        goSetWithoutSend(KO_DIMMER, brightness);
        for (std::list<IDimmerBridge *>::iterator it = dimmerBridges->begin(); it != dimmerBridges->end(); ++it)
        {
            (*it)->setBrightness(brightness);
        }
    }
}

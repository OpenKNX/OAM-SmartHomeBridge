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


KnxChannelDimmer::KnxChannelDimmer(std::list<DimmerBridge *> *dimmerBridges, uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      dimmerBridges(dimmerBridges)
{
    for (std::list<DimmerBridge *>::iterator it = dimmerBridges->begin(); it != dimmerBridges->end(); ++it)
         (*it)->initialize(this);
}

void KnxChannelDimmer::commandBrightness(DimmerBridge* dimmerBridge, uint8_t brightness)
{
    logDebugP("%s received changed. Brightness: %d", getName(), brightness);
    if (brightness > 0)
        lastBrighness = brightness;
    uint8_t knxValue = brightness;
    for (std::list<DimmerBridge *>::iterator it = dimmerBridges->begin(); it != dimmerBridges->end(); ++it)
    {
        if ((*it) != dimmerBridge)
        {
            (*it)->setBrightness(brightness);
        }
    }
    koSetWithoutSend(KO_DIMMER_FEEDBACK, knxValue);
    koSet(KO_DIMMER, knxValue, true);
}

void KnxChannelDimmer::commandPower(DimmerBridge* dimmerBridge, bool power)
{
    if (power)
    {
        uint8_t configValue = 0 == (uint8_t) koGet(KO_DIMMER_FEEDBACK) 
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

void KnxChannelDimmer::setup()
{
    koSetWithoutSend(KO_DIMMER, 0);
    koSetWithoutSend(KO_DIMMER_FEEDBACK, 0);
    koSendReadRequest(KO_DIMMER_FEEDBACK);
}

void KnxChannelDimmer::processInputKo(GroupObject &groupObject)
{
    if (isKo(groupObject, KO_DIMMER_FEEDBACK))
    {
        uint8_t brightness = koGet(KO_DIMMER_FEEDBACK);
        if (brightness > 0)
        {
            lastBrighness = brightness;
            if (brightness < 100)
                lastBrighnessLessThan100 = brightness;
        }    
        koSetWithoutSend(KO_DIMMER, brightness);
        for (std::list<DimmerBridge *>::iterator it = dimmerBridges->begin(); it != dimmerBridges->end(); ++it)
        {
            (*it)->setBrightness(brightness);
        }
    }
}

#include <knx.h>
#include "Bridge.h"
#include "KnxChannelJalousie.h"

#define KO_SLAT_POSITION          KoBRI_KO7_, DPT_Scaling
#define KO_SLAT_POSITION_FEEDBACK KoBRI_KO8_, DPT_Scaling

KnxChannelJalousie::KnxChannelJalousie(std::list<IJalousieBridge *> *jalousieBridges, uint16_t channelIndex)
    : KnxChannelRolladen((std::list<IRolladenBridge *> *)jalousieBridges, channelIndex)
{
    for (std::list<IJalousieBridge *>::iterator it = jalousieBridges->begin(); it != jalousieBridges->end(); ++it)
        (*it)->initialize(this);
}

BlindsHandling KnxChannelJalousie::getBlindsHandling()
{
    return (BlindsHandling) ParamBRI_CHJalousieUpDownHandling;
}

bool KnxChannelJalousie::useStop()
{
    return ParamBRI_CHJalousieUseStop;
}

void KnxChannelJalousie::commandSlatPosition(IJalousieBridge* interface, uint8_t slatPosition)
{
    Serial.print(componentName);
    Serial.println(" device receive changed");
    Serial.print("Slat Position: ");
    Serial.println(slatPosition);
    goSet(KO_SLAT_POSITION, slatPosition, true);
    for (std::list<IRolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
    {
        if ((*it) != interface)
            ((IJalousieBridge*)(*it))->setSlatPosition(slatPosition);
    }
}

void KnxChannelJalousie::loop(unsigned long now, bool initalize)
{
    KnxChannelRolladen::loop(now, initalize);
    if (initalize)
    {
        goSetWithoutSend(KO_SLAT_POSITION, 0);
        goSetWithoutSend(KO_SLAT_POSITION_FEEDBACK, 0);
        goSendReadRequest(KO_SLAT_POSITION_FEEDBACK);
    }
}

void KnxChannelJalousie::received(GroupObject &groupObject)
{
    KnxChannelRolladen::received(groupObject);
    if (isGo(groupObject, KO_SLAT_POSITION_FEEDBACK))
    {
        uint8_t slatPosition = goGet(KO_SLAT_POSITION_FEEDBACK);
        goSetWithoutSend(KO_SLAT_POSITION, slatPosition);
        for (std::list<IRolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
        {
            ((IJalousieBridge*)(*it))->setSlatPosition(slatPosition);
        }
    }
}
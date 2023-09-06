#include <knx.h>
#include "knxprod.h"
#include "KnxChannelFan.h"

#define KO_SWITCH             KoBRI_KO1_, DPT_Switch
#define KO_SWITCH_FEEDBACK    KoBRI_KO2_, DPT_Switch
#define KO_AUTOMATIC          KoBRI_KO3_, DPT_Switch
#define KO_AUTOMATIC_FEEDBACK KoBRI_KO4_, DPT_Switch

KnxChannelFan::KnxChannelFan(std::list<FanBridge *> *fanBridges, uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      fanBridges(fanBridges)
{
    for (std::list<FanBridge *>::iterator it = fanBridges->begin(); it != fanBridges->end(); ++it)
        (*it)->initialize(this);
}

const std::string KnxChannelFan::name()
{
    return std::string("Fan");
}

void KnxChannelFan::commandPower(FanBridge *fanBridge, bool power)
{
    logDebugP("Received changed. Power %s", power ? "true" : "false");

    koSet(KO_SWITCH, power, true);
    for (std::list<FanBridge *>::iterator it = fanBridges->begin(); it != fanBridges->end(); ++it)
    {
        if ((*it) != fanBridge)
            (*it)->setPower(power);
    }
}

void KnxChannelFan::commandAutomatic(FanBridge *fanBridge, bool automatic)
{
    logDebugP("Received changed. Automatic %s", automatic ? "true" : "false");
   
    if (ParamBRI_CHFanKoAutomatic == 0)
        koSet(KO_AUTOMATIC, automatic, true);
    else
        koSet(KO_AUTOMATIC, !automatic, true);
    for (std::list<FanBridge *>::iterator it = fanBridges->begin(); it != fanBridges->end(); ++it)
    {
        if ((*it) != fanBridge)
            (*it)->setPower(automatic);
    }
}

void KnxChannelFan::setup()
{
    koSetWithoutSend(KO_SWITCH, 0);
    koSetWithoutSend(KO_SWITCH_FEEDBACK, 0);
    koSendReadRequest(KO_SWITCH_FEEDBACK);

    koSetWithoutSend(KO_AUTOMATIC, ParamBRI_CHFanKoAutomatic == 1);
    koSetWithoutSend(KO_AUTOMATIC_FEEDBACK, ParamBRI_CHFanKoAutomaticFeedback == 1);
    koSendReadRequest(KO_AUTOMATIC_FEEDBACK);
}

void KnxChannelFan::processInputKo(GroupObject &ko)
{
    if (isKo(ko, KO_SWITCH_FEEDBACK))
    {
        bool power = koGet(KO_SWITCH_FEEDBACK);
        koSetWithoutSend(KO_SWITCH, power);
        for (std::list<FanBridge *>::iterator it = fanBridges->begin(); it != fanBridges->end(); ++it)
        {
            (*it)->setPower(power);
        }
    }
    else if (isKo(ko, KO_AUTOMATIC_FEEDBACK))
    {
        bool automatic = koGet(KO_AUTOMATIC_FEEDBACK);
        if (ParamBRI_CHFanKoAutomaticFeedback == 1)
            automatic = !automatic;

        if (ParamBRI_CHFanKoAutomatic == 0)
            koSetWithoutSend(KO_AUTOMATIC, automatic);
        else
            koSetWithoutSend(KO_AUTOMATIC, !automatic);

        for (std::list<FanBridge *>::iterator it = fanBridges->begin(); it != fanBridges->end(); ++it)
        {
            (*it)->setAutomatic(automatic);
        }
    }
}
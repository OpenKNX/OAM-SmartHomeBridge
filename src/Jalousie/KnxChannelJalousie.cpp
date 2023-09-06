#include <knx.h>
#include "knxprod.h"
#include "KnxChannelJalousie.h"

#define KO_SLAT_POSITION          KoBRI_KO7_, DPT_Scaling
#define KO_SLAT_POSITION_FEEDBACK KoBRI_KO8_, DPT_Scaling

KnxChannelJalousie::KnxChannelJalousie(std::list<RolladenBridge *> *jalousieBridges, uint16_t channelIndex)
    : KnxChannelRolladen(jalousieBridges, channelIndex)
{  

}

const std::string KnxChannelJalousie::name()
{
    return std::string("Jalousie");
}

BlindsHandling KnxChannelJalousie::getBlindsHandling()
{
    return (BlindsHandling) ParamBRI_CHJalousieUpDownHandling;
}

bool KnxChannelJalousie::useStop()
{
    return ParamBRI_CHJalousieUseStop == 1;
}

bool KnxChannelJalousie::commandPosition(RolladenBridge* interface, uint8_t position)
{
    uint8_t currentPosition = KnxChannelRolladen::currentPosition();
    bool result = KnxChannelRolladen::commandPosition(interface, position);
    if (result)
    {
        switch (ParamBRI_CHSlatHandling)
        {
            case 1: // Send 100% if close from 0
                if (position > 0 && currentPosition == 0)
                    commandSlatPosition(nullptr, 100);
            break;
            case 2: // Send 100% for closing, 0% for opening
                if (position > currentPosition)
                    commandSlatPosition(nullptr, 100);
                else if (position < currentPosition)
                    commandSlatPosition(nullptr, 0);
            break;
        }
    }
    return result;
}

void KnxChannelJalousie::commandSlatPosition(RolladenBridge* interface, uint8_t slatPosition)
{
    logDebugP("Received changed. Slat Position: %d", slatPosition);
    koSet(KO_SLAT_POSITION, slatPosition, true);
    for (std::list<RolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
    {
        if ((*it) != (RolladenBridge*)interface)
            (*it)->setSlatPosition(slatPosition);
    }
}

void KnxChannelJalousie::setup()
{
    KnxChannelRolladen::setup();
    koSetWithoutSend(KO_SLAT_POSITION, 0);
    koSetWithoutSend(KO_SLAT_POSITION_FEEDBACK, 0);
    koSendReadRequest(KO_SLAT_POSITION_FEEDBACK);
}

void KnxChannelJalousie::processInputKo(GroupObject &ko)
{
    KnxChannelRolladen::processInputKo(ko);
    if (isKo(ko, KO_SLAT_POSITION_FEEDBACK))
    {
        uint8_t slatPosition = koGet(KO_SLAT_POSITION_FEEDBACK);
        koSetWithoutSend(KO_SLAT_POSITION, slatPosition);
        for (std::list<RolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
        {
            (*it)->setSlatPosition(slatPosition);
        }
    }
}
#include <knx.h>
#include "knxprod.h"
#include "KnxChannelDisplay.h"

#define KO_TEMPERATURE_FEEDBACK KoBRI_KO1_, DPT_Value_Temp
#define KO_HUMIDITY_FEEDBACK    KoBRI_KO1_, DPT_Value_Humidity
#define KO_LUX_FEEDBACK         KoBRI_KO1_, DPT_Value_Lux

KnxChannelDisplay::KnxChannelDisplay(std::list<DisplayBridge *> *displayBridges, uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      displayBridges(displayBridges)
{
    for (std::list<DisplayBridge *>::iterator it = displayBridges->begin(); it != displayBridges->end(); ++it)
         (*it)->initialize(this);
}

DisplayType KnxChannelDisplay::getDisplayType()
{
    return (DisplayType) ParamBRI_CHDisplayType;
}

void KnxChannelDisplay::setup()
{
    switch (getDisplayType())
    {
        case DisplayType::DisplayTypeTemperature:
            koSetWithoutSend(KO_TEMPERATURE_FEEDBACK, 0);
            koSendReadRequest(KO_TEMPERATURE_FEEDBACK);
            break;
        case DisplayType::DisplayTypeHumidity:
            koSetWithoutSend(KO_HUMIDITY_FEEDBACK, 0);
            koSendReadRequest(KO_HUMIDITY_FEEDBACK);
            break;
            case DisplayType::DisplayTypeLux:
            koSetWithoutSend(KO_LUX_FEEDBACK, 0);
            koSendReadRequest(KO_LUX_FEEDBACK);
            break;
    }
}

void KnxChannelDisplay::processInputKo(GroupObject &groupObject)
{
    if (isKo(groupObject, KoBRI_KO1_))
    {
        double value = 0;
        switch (getDisplayType())
        {
            case DisplayType::DisplayTypeTemperature:
                value = koGet(KO_TEMPERATURE_FEEDBACK);
                break;
            case DisplayType::DisplayTypeHumidity:
                value = koGet(KO_HUMIDITY_FEEDBACK);
                break;
            case DisplayType::DisplayTypeLux:
                value = koGet(KO_LUX_FEEDBACK);
                break;
        }
        for (std::list<DisplayBridge *>::iterator it = displayBridges->begin(); it != displayBridges->end(); ++it)
        {
            (*it)->setValue(value);
        }
    }
}

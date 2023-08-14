#include <knx.h>
#include "Bridge.h"
#include "KnxChannelDisplay.h"

#define KO_TEMPERATURE_FEEDBACK KoBRI_KO1_, DPT_Value_Temp
#define KO_HUMIDITY_FEEDBACK    KoBRI_KO1_, DPT_Value_Humidity
#define KO_LUX_FEEDBACK         KoBRI_KO1_, DPT_Value_Lux

KnxChannelDisplay::KnxChannelDisplay(std::list<IDisplayBridge *> *displayBridges, uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      displayBridges(displayBridges)
{
    for (std::list<IDisplayBridge *>::iterator it = displayBridges->begin(); it != displayBridges->end(); ++it)
         (*it)->initialize(this);
}

DisplayType KnxChannelDisplay::getDisplayType()
{
    return (DisplayType) ParamBRI_CHDisplayType;
}

void KnxChannelDisplay::loop(unsigned long now, bool initalize)
{
    if (initalize)
    {
        switch (getDisplayType())
        {
            case DisplayType::DisplayTypeTemperature:
                goSetWithoutSend(KO_TEMPERATURE_FEEDBACK, 0);
                goSendReadRequest(KO_TEMPERATURE_FEEDBACK);
                break;
            case DisplayType::DisplayTypeHumidity:
                goSetWithoutSend(KO_HUMIDITY_FEEDBACK, 0);
                goSendReadRequest(KO_HUMIDITY_FEEDBACK);
                break;
              case DisplayType::DisplayTypeLux:
                goSetWithoutSend(KO_LUX_FEEDBACK, 0);
                goSendReadRequest(KO_LUX_FEEDBACK);
                break;
        }
    }
}

void KnxChannelDisplay::received(GroupObject &groupObject)
{
    if (isGo(groupObject, KoBRI_KO1_))
    {
        double value = 0;
        switch (getDisplayType())
        {
            case DisplayType::DisplayTypeTemperature:
                value = goGet(KO_TEMPERATURE_FEEDBACK);
                break;
            case DisplayType::DisplayTypeHumidity:
                value = goGet(KO_HUMIDITY_FEEDBACK);
                break;
            case DisplayType::DisplayTypeLux:
                value = goGet(KO_LUX_FEEDBACK);
                break;
        }
        for (std::list<IDisplayBridge *>::iterator it = displayBridges->begin(); it != displayBridges->end(); ++it)
        {
            (*it)->setValue(value);
        }
    }
}

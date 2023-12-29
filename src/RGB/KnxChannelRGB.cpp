#include <knx.h>
#include "knxprod.h"
#include "KnxChannelRGB.h"

#define KO_RGB             KoBRI_KO1_, DPT_Colour_RGB
#define KO_RGB_FEEDBACK    KoBRI_KO2_, DPT_Colour_RGB
#define KO_POWER           KoBRI_KO3_, DPT_Switch
#define KO_POWER_FEEDBACK  KoBRI_KO4_, DPT_Switch


enum RGBSwitchBehavior
{
    White = 0,
    Dark = 1,
    Red = 2,
    Green = 3,
    Blue = 4,
    Yellow = 5,
    Magenta = 6,
    Cyan = 7,
    SendPower = 200,
    LastBrightness = 201,
    LastBrightnessLessOtherThenWhite = 202
};


KnxChannelRGB::KnxChannelRGB(DynamicPointerArray<RGBBridge> *RGBBridges, uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      RGBBridges(RGBBridges)
{
    for (auto it = RGBBridges->begin(); it != RGBBridges->end(); ++it)
         (*it)->initialize(this);
}

const std::string KnxChannelRGB::name()
{
    return std::string("RGB");
}

void KnxChannelRGB::commandRGB(RGBBridge* RGBBridge, uint32_t rgb)
{
    logDebugP("Received changed. RGB: %d", rgb);
    if (rgb > 0)
    {
        lastColor = rgb;
        if (rgb != 0xFFFFFF)
            lastColorLessOtherThanWhite = rgb;
    }
    uint32_t knxValue = rgb;
    for (auto it = RGBBridges->begin(); it != RGBBridges->end(); ++it)
    {
        if ((*it) != RGBBridge)
        {
            (*it)->setRGB(rgb);
        }
    }
    if (knxValue == 0)
    {
        koSetWithoutSend(KO_POWER_FEEDBACK, false);
        koSet(KO_POWER, true, true);
        if (!ParamBRI_CHLightRGBUseSwitchKO)
        {
            koSetWithoutSend(KO_RGB_FEEDBACK, 0x000000);
            koSet(KO_RGB, 0x000000, true);
        };
    }
    else
    {
        koSetWithoutSend(KO_RGB_FEEDBACK, knxValue);
        if (ParamBRI_CHLightRGBUseSwitchKO)
            koSet(KO_RGB, knxValue, true);
        koSetWithoutSend(KO_POWER_FEEDBACK, true);
        koSetWithoutSend(KO_POWER, true);
    }
}

void KnxChannelRGB::commandPower(RGBBridge* RGBBridge, bool power)
{
    if (power)
    {
        uint32_t configValue = 0 == (uint32_t) koGet(KO_RGB_FEEDBACK) 
            ? ParamBRI_CHLightRGBSwitchOnBehavior 
            : ParamBRI_CHLightRGBSwitchOn2Behavior;
        switch((RGBSwitchBehavior) configValue)
        {
            case RGBSwitchBehavior::SendPower:      
                if (ParamBRI_CHLightRGBUseSwitchKO)
                {
                    koSetWithoutSend(KO_POWER_FEEDBACK, true);
                    koSet(KO_POWER, true, true);    
                }
                else
                {
                    commandRGB(nullptr, 0xFFFFFF);
                }
                break;
            case RGBSwitchBehavior::White:
                commandRGB(nullptr, 0xFFFFFF);
                break;
            case RGBSwitchBehavior::Dark:
                commandRGB(nullptr, 0x808080);
                break;
            case RGBSwitchBehavior::Red:
                commandRGB(nullptr, 0xFF0000);
                break;
            case RGBSwitchBehavior::Green:
                commandRGB(nullptr, 0x00FF00);
                break;
            case RGBSwitchBehavior::Blue:
                commandRGB(nullptr, 0x0000FF);
                break;
            case RGBSwitchBehavior::Yellow:
                commandRGB(nullptr, 0xFFFF00);
                break;
            case RGBSwitchBehavior::Magenta:
                commandRGB(nullptr, 0xFF00FF);
                break;
            case RGBSwitchBehavior::Cyan:
                commandRGB(nullptr, 0x00FFFF);
                break;
            case RGBSwitchBehavior::LastBrightness:
                commandRGB(nullptr, lastColor);
                break;
            case RGBSwitchBehavior::LastBrightnessLessOtherThenWhite:
                commandRGB(nullptr, lastColorLessOtherThanWhite);
                break;
        }
    }
    else
    {
        koSetWithoutSend(KO_POWER_FEEDBACK, false);
        koSet(KO_POWER, false, true);
    }
}

void KnxChannelRGB::setup()
{
    koSetWithoutSend(KO_RGB, 0x000000);
    koSetWithoutSend(KO_RGB_FEEDBACK, 0x000000);
    koSendReadRequest(KO_RGB_FEEDBACK);
    koSetWithoutSend(KO_POWER, false);
    koSetWithoutSend(KO_POWER_FEEDBACK, false);
    koSendReadRequest(KO_RGB_FEEDBACK);
}

void KnxChannelRGB::processInputKo(GroupObject &groupObject)
{
    if (isKo(groupObject, KO_RGB_FEEDBACK))
    {
        uint32_t rgb = koGet(KO_RGB_FEEDBACK);
        if (rgb > 0)
        {
            lastColor = rgb;
            if (rgb != 0xFFFFFF)
                lastColorLessOtherThanWhite = rgb;
        }    
        koSetWithoutSend(KO_RGB, rgb);
        for (auto it = RGBBridges->begin(); it != RGBBridges->end(); ++it)
        {
            (*it)->setRGB(rgb);
        }
    }
    if (isKo(groupObject, KO_POWER_FEEDBACK))
    {
        bool power = koGet(KO_POWER_FEEDBACK);
        koSetWithoutSend(KO_RGB, power);
        for (auto it = RGBBridges->begin(); it != RGBBridges->end(); ++it)
        {
            (*it)->setPower(power);
        }
    }
}

#include <knx.h>
#include "Bridge.h"
#include "KnxChannelDimmer.h"

#define GO_DIMMER           KoBRI_KO1_, DPT_Scaling
#define GO_DIMMER_FEEDBACK  KoBRI_KO2_, DPT_Scaling

KnxChannelDimmer::KnxChannelDimmer(std::list<IDimmerInterface *> *dimmerInterfaces, uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      dimmerInterfaces(dimmerInterfaces)
{
    for (std::list<IDimmerInterface *>::iterator it = dimmerInterfaces->begin(); it != dimmerInterfaces->end(); ++it)
         (*it)->initialize(this);
}

void KnxChannelDimmer::commandBrightness(IDimmerInterface* dimmerInterface, uint8_t brightness)
{
    Serial.print(componentName);
    Serial.println(" device receive changed");
    Serial.print("Brightness: ");
    Serial.println(brightness);
    if (brightness > 0)
        lastBrighness = brightness;
    uint8_t knxValue = brightness;
    for (std::list<IDimmerInterface *>::iterator it = dimmerInterfaces->begin(); it != dimmerInterfaces->end(); ++it)
    {
        if ((*it) != dimmerInterface)
        {
            (*it)->setBrightness(brightness);
        }
    }
    goSetWithoutSend(GO_DIMMER_FEEDBACK, knxValue);
    goSet(GO_DIMMER, knxValue, true);
}

enum DimmerSwitchOnBehavior
{
    LastBrightness,
    Brightness100,
    Brightness90,
    Brightness80,
    Brightness70,
    Brightness60,
    Brightness50,
    Brightness40,
    Brightness30,
    Brightness20,
    Brightness10,
};


enum DimmerSwitchOn2Behavior
{
    On2Brightness100,
    On2Brightness90,
    On2Brightness80,
    On2Brightness70,
    On2Brightness60,
    On2Brightness50,
    On2Brightness40,
    On2Brightness30,
    On2Brightness20,
    On2Brightness10,
};


void KnxChannelDimmer::commandPower(IDimmerInterface* dimmerInterface, bool power)
{
    if (power)
        {
        if (0 == (uint8_t) goGet(GO_DIMMER_FEEDBACK))
        {
            switch((DimmerSwitchOnBehavior) ParamBRI_CHDimmerSwitchOnBehavior)
            {
                case DimmerSwitchOnBehavior::LastBrightness:
                    commandBrightness(NULL, lastBrighness);
                    break;
                case DimmerSwitchOnBehavior::Brightness100:
                    commandBrightness(NULL, 100);
                    break;
                case DimmerSwitchOnBehavior::Brightness90:
                    commandBrightness(NULL, 90);
                    break;
                case DimmerSwitchOnBehavior::Brightness80:
                    commandBrightness(NULL, 80);
                    break;
                case DimmerSwitchOnBehavior::Brightness70:
                    commandBrightness(NULL, 70);
                    break;
                case DimmerSwitchOnBehavior::Brightness60:
                    commandBrightness(NULL, 60);
                    break;
                case DimmerSwitchOnBehavior::Brightness50:
                    commandBrightness(NULL, 50);
                    break;
                case DimmerSwitchOnBehavior::Brightness40:
                    commandBrightness(NULL, 40);
                    break;
                case DimmerSwitchOnBehavior::Brightness30:
                    commandBrightness(NULL, 30);
                    break;
                case DimmerSwitchOnBehavior::Brightness20:
                    commandBrightness(NULL, 20);
                    break;
                case DimmerSwitchOnBehavior::Brightness10:
                    commandBrightness(NULL, 10);
                    break;
            }
        }
        else
        {
            switch((DimmerSwitchOn2Behavior) ParamBRI_CHDimmerSwitchOnBehavior)
            {
                case DimmerSwitchOn2Behavior::On2Brightness100:
                    commandBrightness(NULL, 100);
                    break;
                case DimmerSwitchOn2Behavior::On2Brightness90:
                    commandBrightness(NULL, 90);
                    break;
                case DimmerSwitchOn2Behavior::On2Brightness80:
                    commandBrightness(NULL, 80);
                    break;
                case DimmerSwitchOn2Behavior::On2Brightness70:
                    commandBrightness(NULL, 70);
                    break;
                case DimmerSwitchOn2Behavior::On2Brightness60:
                    commandBrightness(NULL, 60);
                    break;
                case DimmerSwitchOn2Behavior::On2Brightness50:
                    commandBrightness(NULL, 50);
                    break;
                case DimmerSwitchOn2Behavior::On2Brightness40:
                    commandBrightness(NULL, 40);
                    break;
                case DimmerSwitchOn2Behavior::On2Brightness30:
                    commandBrightness(NULL, 30);
                    break;
                case DimmerSwitchOn2Behavior::On2Brightness20:
                    commandBrightness(NULL, 20);
                    break;
                case DimmerSwitchOn2Behavior::On2Brightness10:
                    commandBrightness(NULL, 10);
                    break;
            }
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
        goSetWithoutSend(GO_DIMMER, 0);
        goSetWithoutSend(GO_DIMMER_FEEDBACK, 0);
        goSendReadRequest(GO_DIMMER_FEEDBACK);
    }
}

void KnxChannelDimmer::received(GroupObject &groupObject)
{
    if (isGo(groupObject, GO_DIMMER_FEEDBACK))
    {
        uint8_t brightness = goGet(GO_DIMMER_FEEDBACK);
        if (brightness > 0)
            lastBrighness = brightness;
        goSetWithoutSend(GO_DIMMER, brightness);
        for (std::list<IDimmerInterface *>::iterator it = dimmerInterfaces->begin(); it != dimmerInterfaces->end(); ++it)
        {
            (*it)->setBrightness(brightness);
        }
    }
}

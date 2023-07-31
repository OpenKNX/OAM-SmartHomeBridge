#include <knx.h>
#include "Bridge.h"
#include "KnxChannelDimmer.h"

#define GO_DIMMER KoBRI_Dimmer, DPT_Scaling
#define GO_DIMMER_FEEDBACK KoBRI_DimmerFeedback, DPT_Scaling
#define GO_SWITCH KoBRI_Switch, DPT_Switch
#define GO_SWITCH_FEEDBACK KoBRI_SwitchFeedback, DPT_Switch

KnxChannelDimmer::KnxChannelDimmer(std::list<IDimmerInterface *> *dimmerInterfaces, uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      dimmerInterfaces(dimmerInterfaces)
{
    for (std::list<IDimmerInterface *>::iterator it = dimmerInterfaces->begin(); it != dimmerInterfaces->end(); ++it)
         (*it)->initialize(this);
}

void KnxChannelDimmer::deviceChanged(IDimmerInterface *dimmerInterface)
{
    Serial.print(componentName);
    Serial.println(" device receive changed");
    float brightness = dimmerInterface->getBrightness();
    Serial.print("Brightness: ");
    Serial.println(brightness);
    bool power = dimmerInterface->getPower();
    if (!power)
        brightness = 0;
    Serial.print("Power: ");
    Serial.println(power);
    uint8_t knxValue = brightness;
    goSet(GO_DIMMER, knxValue, true);
    goSet(GO_SWITCH, knxValue > 0, true);
    for (std::list<IDimmerInterface *>::iterator it = dimmerInterfaces->begin(); it != dimmerInterfaces->end(); ++it)
    {
        if ((*it) != dimmerInterface)
        {
            (*it)->setPower(power);
            (*it)->setBrightness(brightness);
        }
    }
}

void KnxChannelDimmer::loop(unsigned long now, bool initalize)
{
    if (initalize)
    {
        goSetWithoutSend(GO_SWITCH, 0);
        goSetWithoutSend(GO_DIMMER, 0);
        goSetWithoutSend(GO_SWITCH_FEEDBACK, 0);
        goSetWithoutSend(GO_DIMMER_FEEDBACK, 0);
        goSendReadRequest(GO_SWITCH_FEEDBACK);
        goSendReadRequest(GO_DIMMER_FEEDBACK);
    }
}

void KnxChannelDimmer::received(GroupObject &groupObject)
{
    if (isGo(groupObject, GO_SWITCH_FEEDBACK))
    {
        bool power = goGet(GO_SWITCH_FEEDBACK);
        goSetWithoutSend(GO_SWITCH, power);
        for (std::list<IDimmerInterface *>::iterator it = dimmerInterfaces->begin(); it != dimmerInterfaces->end(); ++it)
        {
            (*it)->setPower(power);
        }
    }
    if (isGo(groupObject, GO_DIMMER_FEEDBACK))
    {
        uint8_t brightness = goGet(GO_DIMMER_FEEDBACK);
        goSetWithoutSend(GO_DIMMER, brightness);
        for (std::list<IDimmerInterface *>::iterator it = dimmerInterfaces->begin(); it != dimmerInterfaces->end(); ++it)
        {
            (*it)->setBrightness(brightness);
        }
    }
}

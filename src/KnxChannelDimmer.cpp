#include <knx.h>
#include "Bridge.h"
#include "KnxChannelDimmer.h"

#define GO_SWITCH BRI_KoSwitch, "Switch", DPT_Switch
#define GO_SWITCH_FEEDBACK BRI_KoSwitchFeedback, "Switch Feedback", DPT_Switch
#define GO_DIMMER BRI_KoDimmer, "Dimmer", DPT_Scaling
#define GO_DIMMER_FEEDBACK BRI_KoDimmerFeedback, "Dimmer Feedback", DPT_Scaling

KnxChannelDimmer::KnxChannelDimmer(std::list<IDimmerInterface *> *dimmerInterfaces, uint16_t goOffset, uint32_t parameterOffet)
    : KnxChannelBase(goOffset, parameterOffet),
      dimmerInterfaces(dimmerInterfaces)
{
    for (std::list<IDimmerInterface *>::iterator it = dimmerInterfaces->begin(); it != dimmerInterfaces->end(); ++it)
         (*it)->initialize(this);
}

void KnxChannelDimmer::deviceChanged(IDimmerInterface *dimmerInterface)
{
    Serial.print(name);
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

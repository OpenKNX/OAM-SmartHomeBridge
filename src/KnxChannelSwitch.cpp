#include <knx.h>
#include "Bridge.h"
#include "KnxChannelSwitch.h"

#define GO_SWITCH BRI_KoSwitch, "Switch", DPT_Switch
#define GO_SWITCH_FEEDBACK BRI_KoSwitchFeedback, "Switch Feedback", DPT_Switch

KnxChannelSwitch::KnxChannelSwitch(std::list<ISwitchInterface *> *switchInterfaces, uint16_t goOffset, uint32_t parameterOffet)
    : KnxChannelBase(goOffset, parameterOffet),
      switchInterfaces(switchInterfaces)
{
    for (std::list<ISwitchInterface *>::iterator it = switchInterfaces->begin(); it != switchInterfaces->end(); ++it)
        (*it)->initialize(this);
}

void KnxChannelSwitch::deviceChanged(ISwitchInterface *switchInterface)
{
    Serial.print(name);
    Serial.println(" device receive changed");
    bool power = switchInterface->getPower();
    Serial.print("Power: ");
    Serial.println(power);
    goSet(GO_SWITCH, power, true);
    for (std::list<ISwitchInterface *>::iterator it = switchInterfaces->begin(); it != switchInterfaces->end(); ++it)
    {
        if ((*it) != switchInterface)
            (*it)->setPower(power);
    }
}

void KnxChannelSwitch::loop(unsigned long now, bool initalize)
{
    if (initalize)
    {
        goSetWithoutSend(GO_SWITCH, 0);
        goSetWithoutSend(GO_SWITCH_FEEDBACK, 0);
        goSendReadRequest(GO_SWITCH_FEEDBACK);
    }
}

void KnxChannelSwitch::received(GroupObject &groupObject)
{
    if (isGo(groupObject, GO_SWITCH_FEEDBACK))
    {
        bool power = goGet(GO_SWITCH_FEEDBACK);
        goSetWithoutSend(GO_SWITCH, power);
        for (std::list<ISwitchInterface *>::iterator it = switchInterfaces->begin(); it != switchInterfaces->end(); ++it)
        {
            (*it)->setPower(power);
        }
    }
}
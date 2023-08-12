#include <knx.h>
#include "Bridge.h"
#include "KnxChannelSwitch.h"

#define KO_SWITCH           KoBRI_KO1_, DPT_Switch
#define KO_SWITCH_FEEDBACK  KoBRI_KO2_, DPT_Switch

KnxChannelSwitch::KnxChannelSwitch(std::list<ISwitchInterface *> *switchInterfaces, uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      switchInterfaces(switchInterfaces)
{
    for (std::list<ISwitchInterface *>::iterator it = switchInterfaces->begin(); it != switchInterfaces->end(); ++it)
        (*it)->initialize(this);
}

void KnxChannelSwitch::commandPower(ISwitchInterface *switchInterface, bool power)
{
    Serial.print(componentName);
    Serial.println(" device receive changed");
    Serial.print("Power: ");
    Serial.println(power);
    goSet(KO_SWITCH, power, true);
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
        goSetWithoutSend(KO_SWITCH, 0);
        goSetWithoutSend(KO_SWITCH_FEEDBACK, 0);
        goSendReadRequest(KO_SWITCH_FEEDBACK);
    }
}

void KnxChannelSwitch::received(GroupObject &groupObject)
{
    if (isGo(groupObject, KO_SWITCH_FEEDBACK))
    {
        bool power = goGet(KO_SWITCH_FEEDBACK);
        goSetWithoutSend(KO_SWITCH, power);
        for (std::list<ISwitchInterface *>::iterator it = switchInterfaces->begin(); it != switchInterfaces->end(); ++it)
        {
            (*it)->setPower(power);
        }
    }
}
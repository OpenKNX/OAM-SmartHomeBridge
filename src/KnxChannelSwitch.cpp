#include <knx.h>
#include "Bridge.h"
#include "KnxChannelSwitch.h"

#define KO_SWITCH           KoBRI_KO1_, DPT_Switch
#define KO_SWITCH_FEEDBACK  KoBRI_KO2_, DPT_Switch

KnxChannelSwitch::KnxChannelSwitch(std::list<SwitchBridge *> *switchBridges, uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      switchBridges(switchBridges)
{
    for (std::list<SwitchBridge *>::iterator it = switchBridges->begin(); it != switchBridges->end(); ++it)
        (*it)->initialize(this);
}

void KnxChannelSwitch::commandPower(SwitchBridge *switchBridge, bool power)
{
    Serial.print(getName());
    Serial.println(" device receive changed");
    Serial.print("Power: ");
    Serial.println(power);
    koSet(KO_SWITCH, power, true);
    for (std::list<SwitchBridge *>::iterator it = switchBridges->begin(); it != switchBridges->end(); ++it)
    {
        if ((*it) != switchBridge)
            (*it)->setPower(power);
    }
}

void KnxChannelSwitch::setup()
{
    koSetWithoutSend(KO_SWITCH, 0);
    koSetWithoutSend(KO_SWITCH_FEEDBACK, 0);
    koSendReadRequest(KO_SWITCH_FEEDBACK);
}

void KnxChannelSwitch::processInputKo(GroupObject &ko)
{
    if (isKo(ko, KO_SWITCH_FEEDBACK))
    {
        bool power = koGet(KO_SWITCH_FEEDBACK);
        koSetWithoutSend(KO_SWITCH, power);
        for (std::list<SwitchBridge *>::iterator it = switchBridges->begin(); it != switchBridges->end(); ++it)
        {
            (*it)->setPower(power);
        }
    }
}
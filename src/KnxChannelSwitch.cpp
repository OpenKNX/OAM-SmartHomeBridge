#include <knx.h>
#include "Bridge.h"
#include "KnxChannelSwitch.h"

#define KO_SWITCH           KoBRI_KO1_, DPT_Switch
#define KO_SWITCH_FEEDBACK  KoBRI_KO2_, DPT_Switch

KnxChannelSwitch::KnxChannelSwitch(std::list<ISwitchBridge *> *switchBridges, uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      switchBridges(switchBridges)
{
    for (std::list<ISwitchBridge *>::iterator it = switchBridges->begin(); it != switchBridges->end(); ++it)
        (*it)->initialize(this);
}

void KnxChannelSwitch::commandPower(ISwitchBridge *switchBridge, bool power)
{
    Serial.print(getName());
    Serial.println(" device receive changed");
    Serial.print("Power: ");
    Serial.println(power);
    goSet(KO_SWITCH, power, true);
    for (std::list<ISwitchBridge *>::iterator it = switchBridges->begin(); it != switchBridges->end(); ++it)
    {
        if ((*it) != switchBridge)
            (*it)->setPower(power);
    }
}

void KnxChannelSwitch::setup()
{
    goSetWithoutSend(KO_SWITCH, 0);
    goSetWithoutSend(KO_SWITCH_FEEDBACK, 0);
    goSendReadRequest(KO_SWITCH_FEEDBACK);
}

void KnxChannelSwitch::processInputKo(GroupObject &ko)
{
    if (isGo(ko, KO_SWITCH_FEEDBACK))
    {
        bool power = goGet(KO_SWITCH_FEEDBACK);
        goSetWithoutSend(KO_SWITCH, power);
        for (std::list<ISwitchBridge *>::iterator it = switchBridges->begin(); it != switchBridges->end(); ++it)
        {
            (*it)->setPower(power);
        }
    }
}
#include <knx.h>
#include "Bridge.h"
#include "KnxChannelRolladen.h"

#define KO_POSITION               KoBRI_KO1_, DPT_Scaling
#define KO_POSITION_FEEDBACK      KoBRI_KO2_, DPT_Scaling
#define KO_MOVING_DOWN_FEEDBACK   KoBRI_KO3_, DPT_Switch
#define KO_MOVING_UP_FEEDBACK     KoBRI_KO4_, DPT_Switch
#define KO_MOVE_DOWN_UP           KoBRI_KO5_, DPT_UpDown
#define KO_STOP                   KoBRI_KO6_, DPT_Step

KnxChannelRolladen::KnxChannelRolladen(std::list<IRolladenBridge *> *interfaces, uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      interfaces(interfaces)
{
    for (std::list<IRolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
        (*it)->initialize(this);
}

BlindsHandling KnxChannelRolladen::getBlindsHandling()
{
    return (BlindsHandling) ParamBRI_CHRolladenUpDownHandling;
}

bool KnxChannelRolladen::useStop()
{
    return ParamBRI_CHRolladenUseStop;
}

void KnxChannelRolladen::commandPosition(IRolladenBridge* interface, uint8_t position)
{
    Serial.print(componentName);
    Serial.println(" device receive changed");
    Serial.print("Position: ");
    Serial.println(position);
        
    bool sendPosition = true;
    if (position == 0 || position == 100)
    {
        switch (getBlindsHandling())
        {
            case BlindsHandling::BlindsHandlingSendUpAndDown:
                if (position == 100)
                {
                    goSet(KO_MOVE_DOWN_UP, position == 100, true);
                    sendPosition = false;
                };
                break;
            case BlindsHandling::BlindsHandlingSendDown:
                if (position == 100)
                {
                    goSet(KO_MOVE_DOWN_UP, true, true);
                    sendPosition = false;
                };
                break;
            case BlindsHandling::BlindsHandlingSendUp:
                if (position == 0)
                {
                    goSet(KO_MOVE_DOWN_UP, false, true);
                    sendPosition = false;
                };
                break;

        }
    }
    if (sendPosition)
        goSet(KO_POSITION, position, true);
    else
        goSetWithoutSend(KO_POSITION, position);

    for (std::list<IRolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
    {
        if ((*it) != interface)
            (*it)->setPosition(position);
    }
}

void KnxChannelRolladen::commandStop(IRolladenBridge* interface)
{
    if (!useStop())
        return;
    Serial.print(componentName);
    Serial.println(" device receive changed");
    Serial.println("Stop");
    goSet(KO_STOP, true, true);
}

void KnxChannelRolladen::loop(unsigned long now, bool initalize)
{
    if (initalize)
    {
        goSetWithoutSend(KO_POSITION, 0);
        goSetWithoutSend(KO_POSITION_FEEDBACK, 0);
        goSendReadRequest(KO_POSITION_FEEDBACK);
        goSetWithoutSend(KO_MOVING_DOWN_FEEDBACK, false);
        goSendReadRequest(KO_MOVING_DOWN_FEEDBACK);
        goSetWithoutSend(KO_MOVING_UP_FEEDBACK, false);
        goSendReadRequest(KO_MOVING_UP_FEEDBACK);
        goSetWithoutSend(KO_STOP, false);
    }
}

void KnxChannelRolladen::received(GroupObject &groupObject)
{
    if (isGo(groupObject, KO_POSITION_FEEDBACK))
    {
        uint8_t position = goGet(KO_POSITION_FEEDBACK);
        goSetWithoutSend(KO_POSITION, position);
        for (std::list<IRolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
        {
            (*it)->setPosition(position);
        }
    }
    else if (isGo(groupObject, KO_MOVING_DOWN_FEEDBACK) || isGo(groupObject, KO_MOVING_UP_FEEDBACK))
    {
        bool down = goGet(KO_MOVING_DOWN_FEEDBACK);
        bool up = goGet(KO_MOVING_UP_FEEDBACK);
        MoveState value = MoveState::MoveStateHold;
        if (down)
            value == MoveState::MoveStateDown;
        else if(up)
            value == MoveState::MoveStateUp;
    
        for (std::list<IRolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
        {
            (*it)->setMovement(value);
        }
    }
}
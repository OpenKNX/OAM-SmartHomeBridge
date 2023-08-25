#include <knx.h>
#include "knxprod.h"
#include "KnxChannelRolladen.h"

#define KO_POSITION               KoBRI_KO1_, DPT_Scaling
#define KO_POSITION_FEEDBACK      KoBRI_KO2_, DPT_Scaling
#define KO_MOVING_DOWN_FEEDBACK   KoBRI_KO3_, DPT_Switch
#define KO_MOVING_UP_FEEDBACK     KoBRI_KO4_, DPT_Switch
#define KO_MOVE_DOWN_UP           KoBRI_KO5_, DPT_UpDown
#define KO_STOP                   KoBRI_KO6_, DPT_Step

KnxChannelRolladen::KnxChannelRolladen(std::list<RolladenBridge *> *interfaces, uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      interfaces(interfaces)
{
    for (std::list<RolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
        (*it)->initialize(this);
}

const std::string KnxChannelRolladen::name()
{
    return std::string("Rolladen");
}

BlindsHandling KnxChannelRolladen::getBlindsHandling()
{
    return (BlindsHandling) ParamBRI_CHRolladenUpDownHandling;
}

bool KnxChannelRolladen::useStop()
{
    return ParamBRI_CHRolladenUseStop == 1;
}

uint8_t KnxChannelRolladen::currentPosition()
{
    return koGet(KO_POSITION_FEEDBACK);
}

bool KnxChannelRolladen::commandPosition(RolladenBridge* interface, uint8_t position)
{
    logDebugP("Received changed. Position: %d", position);
  
    bool sendPosition = true;
    if (position == 0 || position == 100)
    {
        if (useStop())
        {
            bool down = koGet(KO_MOVING_DOWN_FEEDBACK);
            bool up = koGet(KO_MOVING_UP_FEEDBACK);
            if (up || down)
            {
                koSet(KO_STOP, true, true);
                updatePosition = true;
                return false;
            }
        }

        switch (getBlindsHandling())
        {
            case BlindsHandling::BlindsHandlingSendUpAndDown:
                if (position == 100)
                {
                    koSet(KO_MOVE_DOWN_UP, position == 100, true);
                    sendPosition = false;
                };
                break;
            case BlindsHandling::BlindsHandlingSendDown:
                if (position == 100)
                {
                    koSet(KO_MOVE_DOWN_UP, true, true);
                    sendPosition = false;
                };
                break;
            case BlindsHandling::BlindsHandlingSendUp:
                if (position == 0)
                {
                    koSet(KO_MOVE_DOWN_UP, false, true);
                    sendPosition = false;
                };
                break;

        }
    }
    if (sendPosition)
        koSet(KO_POSITION, position, true);
    else
        koSetWithoutSend(KO_POSITION, position);

    for (std::list<RolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
    {
        if ((*it) != interface)
            (*it)->setPosition(position);
    }
    return true;
}

void KnxChannelRolladen::setup()
{
    koSetWithoutSend(KO_POSITION, 0);
    koSetWithoutSend(KO_POSITION_FEEDBACK, 0);
    koSendReadRequest(KO_POSITION_FEEDBACK);
    koSetWithoutSend(KO_MOVING_DOWN_FEEDBACK, false);
    koSendReadRequest(KO_MOVING_DOWN_FEEDBACK);
    koSetWithoutSend(KO_MOVING_UP_FEEDBACK, false);
    koSendReadRequest(KO_MOVING_UP_FEEDBACK);
    koSetWithoutSend(KO_STOP, false);
}

void KnxChannelRolladen::loop()
{
    if (updatePosition)
    {
        updatePosition = false;
        uint8_t currentPosition = KnxChannelRolladen::currentPosition();
        for (std::list<RolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
        {

            (*it)->setPosition(currentPosition);
        }
    }
}

void KnxChannelRolladen::processInputKo(GroupObject &ko)
{
    if (isKo(ko, KO_POSITION_FEEDBACK))
    {
        uint8_t position = koGet(KO_POSITION_FEEDBACK);
        koSetWithoutSend(KO_POSITION, position);
        for (std::list<RolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
        {
            (*it)->setPosition(position);
        }
    }
    else if (isKo(ko, KO_MOVING_DOWN_FEEDBACK) || isKo(ko, KO_MOVING_UP_FEEDBACK))
    {
        bool down = koGet(KO_MOVING_DOWN_FEEDBACK);
        bool up = koGet(KO_MOVING_UP_FEEDBACK);
        MoveState value = MoveState::MoveStateHold;
        if (down)
        {
            value = MoveState::MoveStateDown;
            koSetWithoutSend(KO_MOVING_UP_FEEDBACK, false);
            logDebugP("Moving down");
        }
        else if(up)
        {
            value = MoveState::MoveStateUp;
            koSetWithoutSend(KO_MOVING_DOWN_FEEDBACK, false);
            logDebugP("Moving up");
        }
        else
        {
            logDebugP("Stopping move");
        }
        
        for (std::list<RolladenBridge *>::iterator it = interfaces->begin(); it != interfaces->end(); ++it)
        {
            (*it)->setMovement(value);
        }
    }
}
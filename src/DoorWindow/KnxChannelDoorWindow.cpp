#include <knx.h>
#include "knxprod.h"
#include "KnxChannelDoorWindow.h"

#define KO_POSITION                   KoBRI_KO1_, DPT_Scaling
#define KO_FEEDBACK_PERCENT           KoBRI_KO2_, DPT_Scaling
#define KO_FEEDBACK_BIT               KoBRI_KO2_, DPT_Switch
#define KO_CLOSING_FEEDBACK           KoBRI_KO3_, DPT_Switch
#define KO_OPENING_FEEDBACK           KoBRI_KO4_, DPT_Switch
#define KO_OPEN_CLOSE                 KoBRI_KO5_, DPT_OpenClose
#define KO_STOP                       KoBRI_KO6_, DPT_Step
#define KO_OBSTRUCTION_DETECTED       KoBRI_KO7_, DPT_Switch

enum KnxChannelDoorWindowFeedback
{
    DoorWindowFeedbackPercentage,
    DoorWindowFeedbackOpened,
    DoorWindowFeedbackClosed
};

KnxChannelDoorWindow::KnxChannelDoorWindow(DynamicPointerArray<DoorWindowBridge> *interfaces, uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      interfaces(interfaces)
{
    for (auto it = interfaces->begin(); it != interfaces->end(); ++it)
        (*it)->initialize(this);
}

const std::string KnxChannelDoorWindow::name()
{
    return std::string("DoorWindow");
}

bool KnxChannelDoorWindow::useStop()
{
    return ParamBRI_CHDoorWindowUseStop == 1;
}

DoorWindowHandling KnxChannelDoorWindow::getDoorWindowHandling()
{
    return (DoorWindowHandling) ParamBRI_CHDoorWindowUpDownHandling;
}

uint8_t KnxChannelDoorWindow::currentPosition()
{
    switch ((KnxChannelDoorWindowFeedback) ParamBRI_CHDoorWindowFeedbackType)
    {
        case KnxChannelDoorWindowFeedback::DoorWindowFeedbackPercentage:
            return koGet(KO_FEEDBACK_PERCENT);
        case KnxChannelDoorWindowFeedback::DoorWindowFeedbackOpened:
            return koGet(KO_FEEDBACK_BIT) ? 100 : 0;
        case KnxChannelDoorWindowFeedback::DoorWindowFeedbackClosed:
            return koGet(KO_FEEDBACK_BIT) ? 0 : 100;
    }
    return 0;
}

bool KnxChannelDoorWindow::commandPosition(DoorWindowBridge* interface, uint8_t position)
{
    if (ParamBRI_CHDoorWindowMotor == 0)
    {
        logDebugP("Received changed. Position: %d -> Ignored because no motor control", position);
        // No motor, ignore command
        updatePosition = true;
        return true;
    }
    logDebugP("Received changed. Position: %d", position);
  
    bool sendPosition = true;
    if (position == 0 || position == 100)
    {
        if (useStop())
        {
            bool down = koGet(KO_CLOSING_FEEDBACK);
            bool up = koGet(KO_OPENING_FEEDBACK);
            if (up || down)
            {
                koSet(KO_STOP, false, true);
                updatePosition = true;
                return true;
            }
        }
         switch (getDoorWindowHandling())
        {
            case DoorWindowHandling::DoorWindowHandlingSendOpenAndClose:
                if (position == 100)
                {
                    koSet(KO_OPEN_CLOSE, position == 100, true);
                    sendPosition = false;
                };
                break;
            case DoorWindowHandling::DoorWindowHandlingSendClose:
                if (position == 100)
                {
                    koSet(KO_OPEN_CLOSE, true, true);
                    sendPosition = false;
                };
                break;
            case DoorWindowHandling::DoorWindowHandlingSendOpen:
                if (position == 0)
                {
                    koSet(KO_OPEN_CLOSE, false, true);
                    sendPosition = false;
                };
                break;

        }
    }
    if (sendPosition)
        koSet(KO_POSITION, position, true);
    else
        koSetWithoutSend(KO_POSITION, position);

    for (auto it = interfaces->begin(); it != interfaces->end(); ++it)
    {
        if ((*it) != interface)
            (*it)->setPosition(position);
    }
    return true;
}

void KnxChannelDoorWindow::setup()
{
    koSetWithoutSend(KO_POSITION, 0);
    if (KnxChannelDoorWindowFeedback::DoorWindowFeedbackPercentage == (KnxChannelDoorWindowFeedback) ParamBRI_CHDoorWindowFeedbackType)
    {
        koSetWithoutSend(KO_FEEDBACK_PERCENT, 0);
        koSendReadRequest(KO_FEEDBACK_PERCENT);
    }
    else
    {
        koSetWithoutSend(KO_FEEDBACK_BIT, false);
        koSendReadRequest(KO_FEEDBACK_BIT);
    }
    koSetWithoutSend(KO_CLOSING_FEEDBACK, false);
    koSendReadRequest(KO_CLOSING_FEEDBACK);
    koSetWithoutSend(KO_OPENING_FEEDBACK, false);
    koSendReadRequest(KO_OPENING_FEEDBACK);
    koSetWithoutSend(KO_STOP, false);
    koSetWithoutSend(KO_OBSTRUCTION_DETECTED, false);
    koSendReadRequest(KO_OBSTRUCTION_DETECTED);
}

void KnxChannelDoorWindow::loop()
{
    if (updatePosition)
    {
        updatePosition = false;
        uint8_t currentPosition = KnxChannelDoorWindow::currentPosition();
        for (auto it = interfaces->begin(); it != interfaces->end(); ++it)
        {

            (*it)->setPosition(currentPosition);
        }
    }
}

void KnxChannelDoorWindow::processInputKo(GroupObject &ko)
{
    if (isKo(ko, KO_FEEDBACK_PERCENT))
    {
        uint8_t position = currentPosition();
        koSetWithoutSend(KO_POSITION, position);
        for (auto it = interfaces->begin(); it != interfaces->end(); ++it)
        {
            (*it)->setPosition(position);
        }
    }
    else if (isKo(ko, KO_OBSTRUCTION_DETECTED))
    {
        bool obstructionDetected = koGet(KO_OBSTRUCTION_DETECTED);
        for (auto it = interfaces->begin(); it != interfaces->end(); ++it)
        {
            (*it)->setObstructionDetected(obstructionDetected);
        }
    }
    else if (isKo(ko, KO_CLOSING_FEEDBACK) || isKo(ko, KO_OPENING_FEEDBACK))
    {
        bool down = koGet(KO_CLOSING_FEEDBACK);
        bool up = koGet(KO_OPENING_FEEDBACK);
        auto value = DoorWindowMoveState::DoorWindowMoveStateHold;
        if (down)
        {
            value = DoorWindowMoveState::DoorWindowMoveStateOpening;
            koSetWithoutSend(KO_OPENING_FEEDBACK, false);
            logDebugP("Moving down");
        }
        else if(up)
        {
            value = DoorWindowMoveState::DoorWindowMoveStateClosing;
            koSetWithoutSend(KO_CLOSING_FEEDBACK, false);
            logDebugP("Moving up");
        }
        else
        {
            logDebugP("Stopping move");
        }
        
        for (auto it = interfaces->begin(); it != interfaces->end(); ++it)
        {
            (*it)->setMovement(value);
        }
    }
}
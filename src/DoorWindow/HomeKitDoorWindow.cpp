#include "HomeKitDoorWindow.h"

HomeKitDoorWindow::HomeKitDoorWindow(int device) :
    device(device)
{
}



void HomeKitDoorWindow::setup(uint8_t _channelIndex)
{
    type = DoorWindowType::Door;
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
    switch (type)
    {
        case DoorWindowType::Door:
            new ServiceImplementationDoor(this);
                currentPosition = new Characteristic::CurrentPosition(100);
                targetPosition = new Characteristic::TargetPosition(100);
                positionState = new Characteristic::PositionState();
                obstructionDetected = new Characteristic::ObstructionDetected();
         case DoorWindowType::Window:
            new ServiceImplementationWindow(this);
                currentPosition = new Characteristic::CurrentPosition(100);
                targetPosition = new Characteristic::TargetPosition(100);
                positionState = new Characteristic::PositionState();
                obstructionDetected = new Characteristic::ObstructionDetected();
         case DoorWindowType::GarageDoor:
            new ServiceImplementationGarageDoor(this);
                currentDoorState = new Characteristic::CurrentDoorState();
                targetDoorState = new Characteristic::TargetDoorState();
                obstructionDetected = new Characteristic::ObstructionDetected();
       break;
    }
}

boolean HomeKitDoorWindow::update()
{
    if (targetPosition != nullptr && targetPosition->updated())
    {
        return _channel->commandPosition(this, 100 - targetPosition->getNewVal());
    }
    if (targetDoorState != nullptr && targetDoorState->updated())
    {
        return _channel->commandPosition(this, targetDoorState->getNewVal() == 0 ? 100 : 0); // 0 .. open 1..close
    }
    return false;
}


void HomeKitDoorWindow::setPosition(uint8_t position)
{
    if (position < 0)
        position = 0;
    if (position > 100)
        position = 100;
    uint8_t homekitPosition = 100 - position;
    currentPosition->setVal(homekitPosition);
    if (receivedMovement == DoorWindowMoveState::DoorWindowMoveStateHold)
    {
        if (targetDoorState != nullptr)
            targetDoorState->setVal(homekitPosition == 0 ? 0 : 1); // 0 .. open 1..close
        if (targetPosition != nullptr)
            targetPosition->setVal(homekitPosition);
    }
    else
        receivedTargetPosition = homekitPosition;
}

void HomeKitDoorWindow::setMovement(DoorWindowMoveState movement)
{
    receivedMovement = movement;
    switch (movement)
    {
    case DoorWindowMoveState::DoorWindowMoveStateHold:
        if (positionState != nullptr)
            positionState->setVal(2); 
        if (receivedTargetPosition != NoReceivedTargetPosition)
        {
            if (targetPosition != nullptr)
                targetPosition->setVal(receivedTargetPosition);
            if (targetDoorState != nullptr)
                targetDoorState->setVal(receivedTargetPosition == 0 ?  0 : 1); // 0 .. open 1..close
            if (currentPosition != nullptr)
                currentPosition->setVal(receivedTargetPosition);
            if (currentDoorState != nullptr)
            {
                if (receivedTargetPosition == 0)
                    currentDoorState->setVal(0); // open
                else if (receivedTargetPosition == 100)
                    currentDoorState->setVal(1); // closed
                else
                    currentDoorState->setVal(4); // stopped
            }
            receivedTargetPosition = NoReceivedTargetPosition;
        }
        break;
    case DoorWindowMoveState::DoorWindowMoveStateDown:
        if (currentDoorState != nullptr)
            currentDoorState->setVal(2); // opening
        if (positionState != nullptr)
            positionState->setVal(0); // Seems to have no effect in homekit
        // Workaround:
        if (targetPosition != nullptr && currentPosition != nullptr && targetPosition->getVal() >= currentPosition->getVal())
            targetPosition->setVal(0); // Trigger the homekit display to show closing state
        break;
    case DoorWindowMoveState::DoorWindowMoveStateUp:
        if (currentDoorState != nullptr)
            currentDoorState->setVal(3); // closing
        positionState->setVal(1);  // Seems to have no effect in homekit
        // Workaround:
        if (targetPosition != nullptr && currentPosition != nullptr && targetPosition->getVal() <= currentPosition->getVal())
            targetPosition->setVal(100); // Trigger the homekit display to show opening state
        break; 
    }
}

#include "HomeKitRolladen.h"

HomeKitRolladen::HomeKitRolladen(int device) :
    device(device)
{
}

void HomeKitRolladen::setup(uint8_t _channelIndex)
{
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
    new ServiceImplementation(this);
       currentPosition = new Characteristic::CurrentPosition(100);
       targetPosition = new Characteristic::TargetPosition(100);
       positionState = new Characteristic::PositionState();
}

boolean HomeKitRolladen::update()
{
    if (targetPosition->updated())
    {
        return _channel->commandPosition(this, 100 - targetPosition->getNewVal());
    }
    return false;
}


void HomeKitRolladen::setPosition(uint8_t position)
{
    if (position < 0)
        position = 0;
    if (position > 100)
        position = 100;
    uint8_t homekitPosition = 100 - position;
    currentPosition->setVal(homekitPosition);
    if (receivedMovement == MoveState::MoveStateHold)
    {
        targetPosition->setVal(homekitPosition);
    }
    else
        receivedTargetPosition = homekitPosition;
}

void HomeKitRolladen::setMovement(MoveState movement)
{
    receivedMovement = movement;
    switch (movement)
    {
    case MoveState::MoveStateHold:
        positionState->setVal(2); 
        if (receivedTargetPosition != NoReceivedTargetPosition)
        {
            targetPosition->setVal(receivedTargetPosition);
            currentPosition->setVal(receivedTargetPosition);
            receivedTargetPosition = NoReceivedTargetPosition;
        }
        break;
    case MoveState::MoveStateDown:
        positionState->setVal(0); // Seems to have no effect in homekit
        // Workaround:
        if (targetPosition->getVal() >= currentPosition->getVal())
            targetPosition->setVal(0); // Trigger the homekit display to show closing state
        break;
    case MoveState::MoveStateUp:
        positionState->setVal(1);  // Seems to have no effect in homekit
        // Workaround:
        if (targetPosition->getVal() <= currentPosition->getVal())
            targetPosition->setVal(100); // Trigger the homekit display to show opening state
        break; 
    }
}

#include "HomeKitRolladen.h"

HomeKitRolladen::HomeKitRolladen(int device) :
    device(device)
{
}

void HomeKitRolladen::setup()
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
    Serial.print("Position ");
    Serial.println(position);
    if (position < 0)
        position = 0;
    if (position > 100)
        position = 100;
    currentPosition->setVal(100 - position);
}

void HomeKitRolladen::setMovement(MoveState movement)
{
    switch (movement)
    {
    case MoveState::MoveStateHold:
        positionState->setVal(2);
        break;
    case MoveState::MoveStateDown:
        positionState->setVal(0);
        break;
    case MoveState::MoveStateUp:
        positionState->setVal(1);
        break; 
    }
}

#include "HomeKitRolladen.h"

HomeKitRolladen::HomeKitRolladen(int device) :
    device(device)
{
}

void HomeKitRolladen::initialize(KnxChannelRolladen *rolladenDevice)
{
    this->rolladenDevice = rolladenDevice;
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(rolladenDevice->deviceName);
    new ServiceImplementation(this);
       currentPosition = new Characteristic::CurrentPosition();
       targetPosition = new Characteristic::TargetPosition();
       positionState = new Characteristic::PositionState();
}

boolean HomeKitRolladen::update()
{
    if (targetPosition->getVal() != targetPosition->getNewVal())
        rolladenDevice->commandPosition(this, targetPosition->getNewVal());
    return true;
}


void HomeKitRolladen::setPosition(uint8_t position)
{
    currentPosition->setVal(position);
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

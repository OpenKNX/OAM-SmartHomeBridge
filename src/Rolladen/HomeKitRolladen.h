#pragma once
#include "HomeSpan.h"
#include "KnxChannelRolladen.h"

class HomeKitRolladen : public RolladenBridge
{
private:
    const static uint8_t NoReceivedTargetPosition = 255;

    int device;
    uint8_t receivedTargetPosition = NoReceivedTargetPosition;
    Characteristic::CurrentPosition *currentPosition;
    Characteristic::TargetPosition *targetPosition;
    Characteristic::PositionState *positionState;
    class ServiceImplementation : public Service::WindowCovering
    {
        HomeKitRolladen* parent;
        virtual bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementation(HomeKitRolladen* parent) : parent(parent)
        {
        }
    };
protected:
   MoveState receivedMovement = MoveState::MoveStateHold;
 
public:
    HomeKitRolladen(int device);
    virtual void setup(uint8_t _channelIndex) override;

    virtual boolean update();
    virtual void setPosition(uint8_t position) override;
    virtual void setMovement(MoveState movement) override;
};
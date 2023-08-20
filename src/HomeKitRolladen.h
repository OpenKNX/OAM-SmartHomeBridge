#pragma once
#include "HomeSpan.h"
#include "KnxChannelRolladen.h"

class HomeKitRolladen : public RolladenBridge
{
private:
    int device;
    Characteristic::CurrentPosition *currentPosition;
    Characteristic::TargetPosition *targetPosition;
    Characteristic::PositionState *positionState;
    class ServiceImplementation : Service::WindowCovering
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

public:
    HomeKitRolladen(int device);
    virtual void setup() override;

    virtual boolean update();
    virtual void setPosition(uint8_t position) override;
    virtual void setMovement(MoveState movement) override;
};
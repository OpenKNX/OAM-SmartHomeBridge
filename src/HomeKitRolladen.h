#pragma once
#include "HomeSpan.h"
#include "KnxChannelRolladen.h"

class HomeKitRolladen : public IRolladenBridge
{
protected:
    KnxChannelRolladen *rolladenDevice;
private:
    int device;
    Characteristic::CurrentPosition *currentPosition;
    Characteristic::TargetPosition *targetPosition;
    Characteristic::PositionState *positionState;
    class ServiceImplementation : Service::WindowCovering
    {
        HomeKitRolladen* parent;
        bool update()
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
    virtual void initialize(KnxChannelRolladen *rolladenDevice);

    virtual boolean update();
    virtual void setPosition(uint8_t position);
    virtual void setMovement(MoveState movement);
};
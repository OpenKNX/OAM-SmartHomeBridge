#pragma once
#include "HomeSpan.h"
#include "KnxChannelDoorWindow.h"

enum DoorWindowType
{
    Window = 90,
    Door = 91,
    GarageDoor = 92
};

class HomeKitDoorWindow : public DoorWindowBridge
{
private:
    const static uint8_t NoReceivedTargetPosition = 255;

    int device;
    uint8_t receivedTargetPosition = NoReceivedTargetPosition;
    Characteristic::CurrentPosition *currentPosition = nullptr;
    Characteristic::TargetPosition *targetPosition = nullptr;
    Characteristic::PositionState *positionState = nullptr;
    Characteristic::CurrentDoorState *currentDoorState = nullptr;
    Characteristic::TargetDoorState *targetDoorState = nullptr;
    Characteristic::ObstructionDetected *obstructionDetected = nullptr;
    DoorWindowType type;

    class ServiceImplementationDoor : public Service::Door
    {
        HomeKitDoorWindow* parent;
        virtual bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementationDoor(HomeKitDoorWindow* parent) : parent(parent)
        {
        }
    };
    class ServiceImplementationWindow : public Service::Window
    {
        HomeKitDoorWindow* parent;
        virtual bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementationWindow(HomeKitDoorWindow* parent) : parent(parent)
        {
        }
    };
    class ServiceImplementationGarageDoor : public Service::GarageDoorOpener
    {
        HomeKitDoorWindow* parent;
        virtual bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementationGarageDoor(HomeKitDoorWindow* parent) : parent(parent)
        {
        }
    };
protected:
   DoorWindowMoveState receivedMovement = DoorWindowMoveState::DoorWindowMoveStateHold;
 
public:
    HomeKitDoorWindow(int device);
    virtual void setup(uint8_t _channelIndex) override;

    virtual boolean update();
    virtual void setPosition(uint8_t position) override;
    virtual void setMovement(DoorWindowMoveState movement) override;
    virtual void setObstructionDetected(bool obstructionDetected) override;
};
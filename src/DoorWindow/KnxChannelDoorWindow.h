#pragma once
#include "KnxChannelBase.h"

class KnxChannelDoorWindow;

enum DoorWindowMoveState
{
    DoorWindowMoveStateHold,
    DoorWindowMoveStateClosing,
    DoorWindowMoveStateOpening
};

enum DoorWindowHandling
{
    DoorWindowHandlingNothing,
    DoorWindowHandlingSendClose,
    DoorWindowHandlingSendOpen,
    DoorWindowHandlingSendOpenAndClose
};

class DoorWindowBridge : public ChannelBridgeBase<KnxChannelDoorWindow>
{
public:
    virtual void setPosition(uint8_t position) = 0;
    virtual void setMovement(DoorWindowMoveState movement) = 0;
    virtual void setObstructionDetected(bool obstructionDetected) = 0;
};

class KnxChannelDoorWindow : public KnxChannelBase
{
    public:
        std::list<DoorWindowBridge*>* interfaces; 
        KnxChannelDoorWindow(std::list<DoorWindowBridge*>* interfaces, uint16_t channelIndex);
    protected:
        volatile bool updatePosition = false;

        virtual void setup() override;
        virtual void loop() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual bool useStop();
        virtual DoorWindowHandling getDoorWindowHandling();
        uint8_t currentPosition();

    public:
        virtual bool commandPosition(DoorWindowBridge* interface, uint8_t position);
        virtual const std::string name() override;
};

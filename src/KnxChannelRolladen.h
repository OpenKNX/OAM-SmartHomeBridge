#pragma once
#include "KnxChannelBase.h"

class KnxChannelRolladen;

enum MoveState
{
    MoveStateHold,
    MoveStateUp,
    MoveStateDown
};

enum BlindsHandling
{
    BlindsHandlingNothing,
    BlindsHandlingSendDown,
    BlindsHandlingSendUp,
    BlindsHandlingSendUpAndDown
};

class IRolladenBridge
{
    public:
    virtual void initialize(KnxChannelRolladen* rolladenDevice) = 0;
    virtual void setPosition(uint8_t position) = 0;
    virtual void setMovement(MoveState movement) = 0;
};

class KnxChannelRolladen : public KnxChannelBase
{
    public:
        std::list<IRolladenBridge*>* interfaces; 
        KnxChannelRolladen(std::list<IRolladenBridge*>* interfaces, uint16_t channelIndex);
    protected:
        volatile bool updatePosition = false;

        virtual void setup() override;
        virtual void loop() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual BlindsHandling getBlindsHandling();
        virtual bool useStop();
        uint8_t currentPosition();

    public:
        virtual bool commandPosition(IRolladenBridge* interface, uint8_t position);
};

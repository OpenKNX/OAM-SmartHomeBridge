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
    virtual void initialize(KnxChannelRolladen* switchDevice) = 0;
    virtual void setPosition(uint8_t position) = 0;
    virtual void setMovement(MoveState movement) = 0;
};

class KnxChannelRolladen : public KnxChannelBase
{
    public:
        std::list<IRolladenBridge*>* interfaces; 
        KnxChannelRolladen(std::list<IRolladenBridge*>* interfaces, uint16_t channelIndex);
    protected:
        virtual void loop(unsigned long now, bool initalize);
        virtual void received(GroupObject& groupObject);
        virtual BlindsHandling getBlindsHandling();
        virtual bool useStop();

    public:
        void commandPosition(IRolladenBridge* interface, uint8_t position);
        void commandStop(IRolladenBridge* interface);
};

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

class RolladenBridge : public ChannelBridgeBase<KnxChannelRolladen>
{
public:
    virtual void setPosition(uint8_t position) = 0;
    virtual void setMovement(MoveState movement) = 0;
    virtual void setSlatPosition(uint8_t slatPosition){}
};

class KnxChannelRolladen : public KnxChannelBase
{
    public:
        std::list<RolladenBridge*>* interfaces; 
        KnxChannelRolladen(std::list<RolladenBridge*>* interfaces, uint16_t channelIndex);
    protected:
        volatile bool updatePosition = false;

        virtual void setup() override;
        virtual void loop() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual BlindsHandling getBlindsHandling();
        virtual bool useStop();
        uint8_t currentPosition();

    public:
        virtual bool commandPosition(RolladenBridge* interface, uint8_t position);
        virtual const std::string name() override;
};

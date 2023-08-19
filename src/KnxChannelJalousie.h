#pragma once
#include "KnxChannelRolladen.h"

class KnxChannelJalousie;


class IJalousieBridge : public IRolladenBridge
{
    public:
    virtual void initialize(KnxChannelJalousie* switchDevice) = 0;
    virtual void setSlatPosition(uint8_t slatPosition) = 0;
};

class KnxChannelJalousie : public KnxChannelRolladen
{
    public:
        KnxChannelJalousie(std::list<IJalousieBridge*>* jalousieBridges, uint16_t channelIndex);
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual BlindsHandling getBlindsHandling() override;
        virtual bool useStop() override;

    public:
        virtual bool commandPosition(IRolladenBridge* interface, uint8_t position) override;
        void commandSlatPosition(IJalousieBridge* interface, uint8_t slatPosition);
};

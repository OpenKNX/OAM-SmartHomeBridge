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
        virtual void loop(unsigned long now, bool initalize);
        virtual void received(GroupObject& groupObject);
        virtual BlindsHandling getBlindsHandling();
        virtual bool useStop();

    public:
        virtual bool commandPosition(IRolladenBridge* interface, uint8_t position);
        void commandSlatPosition(IJalousieBridge* interface, uint8_t slatPosition);
};

#pragma once
#include "KnxChannelRolladen.h"

class KnxChannelJalousie;

class KnxChannelJalousie : public KnxChannelRolladen
{
    public:
        KnxChannelJalousie(std::list<RolladenBridge*>* jalousieBridges, uint16_t channelIndex);
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual BlindsHandling getBlindsHandling() override;
        virtual bool useStop() override;

    public:
        virtual bool commandPosition(RolladenBridge* interface, uint8_t position) override;
        void commandSlatPosition(RolladenBridge* interface, uint8_t slatPosition);
};

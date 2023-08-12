#pragma once
#include "KnxChannelBase.h"

class KnxChannelDimmer;

class IDimmerInterface
{
    public:
    virtual void initialize(KnxChannelDimmer* dimmerDevice) = 0;
    virtual void setBrightness(uint8_t brightness) = 0;
};

class KnxChannelDimmer : public KnxChannelBase
{
    public:
        std::list<IDimmerInterface *> *dimmerInterfaces;
        KnxChannelDimmer(std::list<IDimmerInterface *> *dimmerInterfaces, uint16_t channelIndex);
    protected:
        uint8_t lastBrighness = 100;
        virtual void loop(unsigned long now, bool initalize);
        virtual void received(GroupObject& groupObject);

        public:
            void commandPower(IDimmerInterface* dimmerInterface, bool on);
            void commandBrightness(IDimmerInterface* dimmerInterface, uint8_t brightness);
};

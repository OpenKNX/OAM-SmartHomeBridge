#pragma once
#include "KnxChannelBase.h"

class KnxChannelDimmer;

class IDimmerInterface
{
    public:
    virtual void initialize(KnxChannelDimmer* dimmerDevice) = 0;
    virtual void setBrightness(int brightness) = 0;
    virtual void setPower(bool on) = 0;
    virtual bool getPower() = 0; 
    virtual int getBrightness() = 0;  
};

class KnxChannelDimmer : public KnxChannelBase
{
    public:
        std::list<IDimmerInterface *> *dimmerInterfaces;
        KnxChannelDimmer(std::list<IDimmerInterface *> *dimmerInterfaces, uint16_t goOffset, uint32_t parameterOffset);
    protected:
        virtual void loop(unsigned long now, bool initalize);
        virtual void received(GroupObject& groupObject);

        public:
            void deviceChanged(IDimmerInterface* dimmerInterface);
};

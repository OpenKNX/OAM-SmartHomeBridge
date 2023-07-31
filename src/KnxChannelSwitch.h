#pragma once
#include "KnxChannelBase.h"

class KnxChannelSwitch;

class ISwitchInterface
{
    public:
    virtual void initialize(KnxChannelSwitch* switchDevice) = 0;
    virtual void setPower(bool on) = 0;
    virtual bool getPower() = 0; 
};

class KnxChannelSwitch : public KnxChannelBase
{
    public:
        std::list<ISwitchInterface*>* switchInterfaces; 
        KnxChannelSwitch(std::list<ISwitchInterface*>* switchInterfaces, uint16_t channelIndex);
    protected:
        virtual void loop(unsigned long now, bool initalize);
        virtual void received(GroupObject& groupObject);

        public:
            void deviceChanged(ISwitchInterface* switchInterface);
};

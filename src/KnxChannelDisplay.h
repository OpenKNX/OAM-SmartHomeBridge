#pragma once
#include "KnxChannelBase.h"

class KnxChannelDisplay;

class IDisplayBridge
{
    public:
    virtual void initialize(KnxChannelDisplay* displayDevice) = 0;
    virtual void setValue(double value) = 0;
};

enum DisplayType
{
    DisplayTypeTemperature,
    DisplayTypeHumidity,
    DisplayTypeLux,
};

class KnxChannelDisplay : public KnxChannelBase
{
    public:
        std::list<IDisplayBridge *> *displayBridges;
        KnxChannelDisplay(std::list<IDisplayBridge *> *displayBridges, uint16_t channelIndex);
        DisplayType getDisplayType();
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

};

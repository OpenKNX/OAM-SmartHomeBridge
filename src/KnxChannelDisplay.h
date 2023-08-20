#pragma once
#include "KnxChannelBase.h"

class KnxChannelDisplay;

class DisplayBridge : public ChannelBridgeBase<KnxChannelDisplay>
{
public:
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
        std::list<DisplayBridge *> *displayBridges;
        KnxChannelDisplay(std::list<DisplayBridge *> *displayBridges, uint16_t channelIndex);
        DisplayType getDisplayType();
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

};

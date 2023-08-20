#pragma once
#include "component.h"
#include "OpenKNX.h"

template<class T> 
class ChannelBridgeBase
{
protected:
    T* _channel;
public:
    virtual void initialize(T* channel)
    {
        _channel = channel;
        setup();
    }
protected:
    virtual void setup()
    {
    }
};

class KnxChannelBase : public OpenKNX::Channel, public Component
{
    private:
        const char* utf8Name = NULL;
    public:
        ~KnxChannelBase();
        KnxChannelBase(uint16_t channelIndex);
        virtual const std::string logPrefix() override;
        virtual const std::string name() override;
        const char* getNameInUTF8();
};
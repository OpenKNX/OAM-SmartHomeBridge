#pragma once
#include "component.h"
#include "OpenKNX.h"

template<class T> 
class ChannelBridgeBase
{
protected:
    T* _channel = NULL;
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

    const std::string logPrefix()
    {
        if (_channel == NULL)
            return "Not initialized ChannelBridge";
        return _channel->logPrefix();
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
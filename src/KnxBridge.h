
#pragma once
#include "OpenKNX.h"
#include "ChannelOwnerModule.h"
#include "Component.h"

class KnxBridge;
class HueBridge;

class BridgeBase : public OpenKNX::Base
{
    public:
    virtual void initialize(KnxBridge* bridge) = 0;
    virtual void loop() = 0;
    virtual void processInputKo(GroupObject& ko) = 0;
};

enum Mode
{
    Homekit = 1,
    HueBridgeEmulation = 2
};

class KnxBridge : public ChannelOwnerModule
{
    private:
        const char* _utf8Name = NULL;
        HueBridge* _pHueBridge = NULL;
        std::list<BridgeBase*>* bridgeInterfaces = NULL;
    protected:
        virtual void setup() override;
        virtual void loop() override;
        virtual void processInputKo(GroupObject &ko) override;
        virtual OpenKNX::Channel* createChannel(uint8_t _channelIndex /* this parameter is used in macros, do not rename */); 
    public:
        KnxBridge();
        ~KnxBridge();
        const char* getNameInUTF8();
};

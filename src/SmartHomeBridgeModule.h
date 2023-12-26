#pragma once
#include "OpenKNX.h"
#include "ChannelOwnerModule.h"
#include "Component.h"
#include <WebServer.h>

#if !defined(ARDUINO_ARCH_ESP32)
#error Your archetecture is not supported by the smart home brigde. Currently only ESP32 is supported.
#endif

class SmartHomeBridgeModule;
class HueBridge;

class BridgeBase : public OpenKNX::Base
{
public:
    virtual void initialize(SmartHomeBridgeModule* bridge) = 0;
    virtual void start(SmartHomeBridgeModule* bridge) {};
    virtual void loop() = 0;
    virtual void processInputKo(GroupObject& ko) = 0;
    virtual void getInformation(String& result) = 0;
};

enum Mode
{
    Homekit = 1,
    HueBridgeEmulation = 2
};

class SmartHomeBridgeModule : public ChannelOwnerModule
{
    private:
        const char* _utf8Name = nullptr;
        HueBridge* _pHueBridge = nullptr;
        DynamicPointerArray<BridgeBase>* bridgeInterfaces = nullptr;
        WebServer* webServer = nullptr;
        volatile bool started = false;
    protected:
        virtual const std::string name() override;
        virtual const std::string version() override;
 
        virtual void setup() override;
        virtual void loop() override;
#ifdef OPENKNX_DUALCORE
        virtual void loop1() override;
#endif
        virtual void processInputKo(GroupObject &ko) override;
        virtual OpenKNX::Channel* createChannel(uint8_t _channelIndex /* this parameter is used in macros, do not rename */); 
    public:
        SmartHomeBridgeModule();
        ~SmartHomeBridgeModule();
        const char* getNameInUTF8();
        WebServer* getWebServer();
        void serveHomePage();
        void serveFirmwareUpdatePage();
};

extern SmartHomeBridgeModule openknxSmartHomeBridgeModule;
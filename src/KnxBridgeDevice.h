#pragma once
#include "component.h"

class KnxBridgeDevice;

class IBridgeInterface
{
    public:
    virtual void initialize(KnxBridgeDevice* brigdeDevice) = 0;
    virtual void loop() = 0;
};
enum Mode
{
    Homekit = 1,
    HueBridgeEmulation = 2
};

class KnxBridgeDevice : Component
{
    private:
        std::list<IBridgeInterface *> *bridgeInterfaces = NULL;
    public:
        
        char ssid[32 + 1]; // One more then chars for ending 0
        char password[64 + 1]; // One more then chars for ending 0

        char deviceName[25 + 1]; // One more then chars for ending 0
        Mode mode;
        char pairingCode[8 + 1]; // One more then chars for ending 0
        KnxBridgeDevice();
        void initialize(std::list<IBridgeInterface *> *bridgeInterfaces);
   protected:
        virtual void loop(unsigned long now, bool initalize);
 
};

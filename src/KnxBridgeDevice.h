#pragma once
#include "component.h"

class KnxBridgeDevice;

class IBridge
{
    public:
    virtual void initialize(KnxBridgeDevice* brigdeDevice) = 0;
    virtual void loop() = 0;
    virtual void received(GroupObject& groupObject) = 0;
};

enum Mode
{
    Homekit = 1,
    HueBridgeEmulation = 2
};

class KnxBridgeDevice : public Component
{
    private:
        std::list<IBridge *> *bridgeInterfaces = NULL;
    public:
        KnxBridgeDevice();
        void initialize(std::list<IBridge *> *bridgeInterfaces);
   protected:
        virtual void loop(unsigned long now, bool initalize);
        virtual void received(GroupObject& groupObject);
};

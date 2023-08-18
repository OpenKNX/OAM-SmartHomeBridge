
#pragma once
#include "OpenKNX.h"
#include "Component.h"

class KnxBridge;

class IBridge
{
    public:
    virtual void initialize(KnxBridge* bridge) = 0;
    virtual void loop() = 0;
    virtual void received(GroupObject& groupObject) = 0;
};

enum Mode
{
    Homekit = 1,
    HueBridgeEmulation = 2
};

class KnxBridge : public OpenKNX::Module
{
    private:
        const char* utf8Name = NULL;
    private:
        std::list<IBridge *> *bridgeInterfaces = NULL;
        bool _initalize = true;
        std::list<Component*> _components;   
    protected:
        virtual void setup();
        virtual void processInputKo(GroupObject &ko);
        virtual void loop();
    public:
        ~KnxBridge();
        const char* getNameInUTF8();

};
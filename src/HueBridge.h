#pragma once
#include "HomeSpan.h"
#include <Espalexa.h>
#include "SmartHomeBridgeModule.h"

class HueBridge : public BridgeBase
{
public:
    Espalexa espalexa;
private:
    volatile bool started = false; 
public:
    virtual void initialize(SmartHomeBridgeModule *bridge) override;
    virtual void start(SmartHomeBridgeModule *bridge) override;
    virtual void loop() override;
#ifdef OPENKNX_DUALCORE
    virtual void loop1() override;
#endif
    virtual void processInputKo(GroupObject& ko) override;
};
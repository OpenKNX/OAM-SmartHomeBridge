#pragma once
#include "HomeSpan.h"
#include <Espalexa.h>
#include "SmartHomeBridgeModule.h"

class HueBridge : public BridgeBase
{
public:
    Espalexa espalexa;
private:
    bool started = false;    
public:
    virtual void initialize(SmartHomeBridgeModule *bridge) override;
    virtual void loop() override;
    virtual void processInputKo(GroupObject& ko) override;
};
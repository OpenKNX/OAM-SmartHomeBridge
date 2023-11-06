#pragma once
#include "HomeSpan.h"
#include <Espalexa.h>
#include "SmartHomeBridgeModule.h"

class HueBridge : public BridgeBase
{
public:
    Espalexa espalexa;
public:
    virtual const std::string name() override;
    virtual void initialize(SmartHomeBridgeModule *bridge) override;
    virtual void start(SmartHomeBridgeModule *bridge) override;
    virtual void loop() override;
    virtual void getInformation(String& result) override;
#ifdef OPENKNX_DUALCORE
    virtual void loop1() override;
#endif
    virtual void processInputKo(GroupObject& ko) override;
};
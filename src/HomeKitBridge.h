#pragma once
#include "HomeSpan.h"
#include "SmartHomeBridgeModule.h"

class HomeKitBridge : public BridgeBase
{
public:
    virtual void initialize(SmartHomeBridgeModule *bridge) override;
    virtual void start(SmartHomeBridgeModule *bridge) override;
    virtual void loop() override;
    virtual void processInputKo(GroupObject& ko) override;
};
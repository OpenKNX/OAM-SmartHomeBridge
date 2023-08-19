#pragma once
#include "HomeSpan.h"
#include "KnxBridge.h"

class HomeKitBridge : public BridgeBase
{
public:
    virtual void initialize(KnxBridge *bridge) override;
    virtual void loop() override;
    virtual void processInputKo(GroupObject& ko) override;
};
#pragma once
#include "HomeSpan.h"
#include "KnxBridgeDevice.h"

class HomeKitBridge : public IBridge
{
public:
    void initialize(KnxBridgeDevice *bridgeDevice);
    void loop();
    void received(GroupObject& groupObject);
};
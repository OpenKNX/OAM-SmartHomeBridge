#pragma once
#include "HomeSpan.h"
#include "KnxBridgeDevice.h"

class HomeKitBridge : public IBridgeInterface
{
public:
    void initialize(KnxBridgeDevice *bridgeDevice);
    void loop();
};
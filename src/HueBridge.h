#pragma once
#include "HomeSpan.h"
#include "KnxBridgeDevice.h"
#include <Espalexa.h>


class HueBridge : public IBridgeInterface
{
public:
    Espalexa espalexa;
private:
    bool started = false;    
public:
    void initialize(KnxBridgeDevice *bridgeDevice);
    void loop();
};
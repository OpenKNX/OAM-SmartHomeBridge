#pragma once
#include "HomeSpan.h"
#include <Espalexa.h>
#include "KnxBridge.h"


class HueBridge : public IBridge
{
public:
    Espalexa espalexa;
private:
    bool started = false;    
public:
    void initialize(KnxBridge *bridge);
    void loop();
    void received(GroupObject& groupObject);
};
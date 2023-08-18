#pragma once
#include "HomeSpan.h"
#include "KnxBridge.h"

class HomeKitBridge : public IBridge
{
public:
    void initialize(KnxBridge *bridge);
    void loop();
    void received(GroupObject& groupObject);
};
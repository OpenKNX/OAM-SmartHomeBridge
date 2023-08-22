#include "HueBridge.h"

void HueBridge::initialize(SmartHomeBridgeModule *bridge)
{
}

void HueBridge::loop()
{
    if (!started && WiFi.status() == WL_CONNECTED)
    {
        started = true;
        espalexa.begin();
    }
    else
    {
        espalexa.loop();
    }
}

void HueBridge::processInputKo(GroupObject& groupObject)
{
    
}



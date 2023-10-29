#include "HueBridge.h"

void HueBridge::initialize(SmartHomeBridgeModule *bridge)
{
}

const std::string HueBridge::name()
{
    return "HueBridge";
}

void HueBridge::start(SmartHomeBridgeModule *bridge)
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

#ifdef OPENKNX_DUALCORE
void HueBridge::loop1()
{
}
#endif

void HueBridge::processInputKo(GroupObject& groupObject)
{
    
}



#include "HueBridge.h"

void HueBridge::initialize(KnxBridgeDevice *bridgeDevice)
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



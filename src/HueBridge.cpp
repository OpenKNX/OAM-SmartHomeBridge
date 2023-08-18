#include "HueBridge.h"

void HueBridge::initialize(KnxBridgeDevice *bridgeDevice)
{
    Serial.println("HueBridge intialized");
}

void HueBridge::loop()
{
    if (!started && WiFi.status() == WL_CONNECTED)
    {
        started = true;
        Serial.println("HueBridge started");
        espalexa.begin();
    }
    else
    {
        espalexa.loop();
    }
}

void HueBridge::received(GroupObject& groupObject)
{
    
}



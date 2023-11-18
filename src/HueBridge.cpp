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
    espalexa.begin(bridge->getWebServer());
}

void HueBridge::loop()
{
    espalexa.loop();
}

#ifdef OPENKNX_DUALCORE
void HueBridge::loop1()
{
}
#endif

void HueBridge::processInputKo(GroupObject& groupObject)
{
    
}


void HueBridge::getInformation(String& result) 
{
    result += "<h3>Hue</h3><a href=\"espalexa\">Information</a>";
}
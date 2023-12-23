#include "HomeKitBridge.h"
#include "SmartHomeBridgeModule.h"

#ifndef HOMESPAN_STACK_SIZE
#define HOMESPAN_STACK_SIZE 9216
#endif

void HomeKitBridge::initialize(SmartHomeBridgeModule *bridge)
{
    homeSpan.setWifiCredentials((const char*)ParamBRI_WiFiSSID, (const char*) ParamBRI_WiFiPassword);
    homeSpan.setPairingCode((const char*)ParamBRI_PairingCode);
    homeSpan.setPortNum(8080);
    homeSpan.begin(Category::Bridges, bridge->getNameInUTF8());
    new SpanAccessory();
    new Service::AccessoryInformation();
    new Characteristic::Identify();
}

const std::string HomeKitBridge::name()
{
    return "HomeKitBridge";
}

void HomeKitBridge::start(SmartHomeBridgeModule *bridge)
{
    BridgeBase::start(bridge);
    homeSpan.autoPoll(HOMESPAN_STACK_SIZE);
}

void HomeKitBridge::loop()
{
   //homeSpan.poll();
}


void HomeKitBridge::processInputKo(GroupObject& ko)
{
    if (ko.asap() == BRI_KoHomeKitFactoryReset && (boolean) ko.value(DPT_Reset))
    {
        homeSpan.processSerialCommand("F");
    }
}

void HomeKitBridge::getInformation(String& result) 
{ 
    result += "<h3>HomeKit</h3>";
    auto minFreeStack = homeSpan.getAutoPollMinFreeStack();
    if (minFreeStack != 0)
    {
        result +="Max Stack Usage: ";
        result += HOMESPAN_STACK_SIZE - minFreeStack;
        result +=" of " + (String) HOMESPAN_STACK_SIZE;
    }
}


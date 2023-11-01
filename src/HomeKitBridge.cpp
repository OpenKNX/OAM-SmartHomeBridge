#include "HomeKitBridge.h"
#include "SmartHomeBridgeModule.h"

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
}

void HomeKitBridge::loop()
{
   homeSpan.poll();
}

void HomeKitBridge::processInputKo(GroupObject& ko)
{
    if (ko.asap() == BRI_KoHomeKitFactoryReset && (boolean) ko.value(DPT_Reset))
    {
        homeSpan.processSerialCommand("F");
    }
}



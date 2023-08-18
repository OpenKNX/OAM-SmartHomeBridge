#include "HomeKitBridge.h"
#include "Bridge.h"

void HomeKitBridge::initialize(KnxBridgeDevice *bridgeDevice)
{
    homeSpan.setWifiCredentials((const char*)ParamBRI_WiFiSSID, (const char*) ParamBRI_WiFiPassword);
    homeSpan.setPairingCode((const char*)ParamBRI_PairingCode);
    homeSpan.setPortNum(8080);
    homeSpan.begin(Category::Bridges, bridgeDevice->getName());
    new SpanAccessory();
    new Service::AccessoryInformation();
    new Characteristic::Identify();
}

void HomeKitBridge::loop()
{
    homeSpan.poll();
}

void HomeKitBridge::received(GroupObject& groupObject)
{
    if (groupObject.asap() == BRI_KoHomeKitFactoryReset && (boolean) groupObject.value(DPT_Reset))
    {
        homeSpan.processSerialCommand("F");
    }
}



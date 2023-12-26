#include "HomeKitBridge.h"
#include "SmartHomeBridgeModule.h"

#ifndef HOMESPAN_STACK_SIZE
#define HOMESPAN_STACK_SIZE 9216
#endif

void HomeKitBridge::initialize(SmartHomeBridgeModule *bridge)
{
    _bridge = bridge;
    homeSpan.setWifiCredentials((const char*)ParamBRI_WiFiSSID, (const char*) ParamBRI_WiFiPassword);
    homeSpan.setPairingCode((const char*)ParamBRI_PairingCode);
    homeSpan.setPortNum(8080);
    homeSpan.begin(Category::Bridges, bridge->getNameInUTF8());
    new SpanAccessory();
    new Service::AccessoryInformation();
    new Characteristic::Identify();
}

void HomeKitBridge::initWebServer(WebServer& webServer)
{
    webServer.on("/resetPairing", HTTP_POST, [=](){serveResetPairingPage();});
}

void HomeKitBridge::serveResetPairingPage()
{
    auto webServer = _bridge->getWebServer();

    String res = "<!DOCTYPE html><html lang=\"en\"><meta charset=\"UTF-8\"><meta http-equiv=\"refresh\" content=\"3;url=/\"><title>";
    res + "HomeKit Pairing Reset";
    res += "</title><body>";
    res += "<br>HomeKit Pairing reseted</br>";
    res += "</body>";
    webServer->send(200, "text/html;charset=UTF-8", res);
    homeSpan.processSerialCommand("F");
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
}

void HomeKitBridge::processInputKo(GroupObject& ko)
{
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
         // HomeKit Factory Reset
        result += "<form method='post' action='/resetPairing'><input name='resetPairing' type='hidden' value='1'><input type='submit' value='Reset Pairing'></form>"; 
    }
}


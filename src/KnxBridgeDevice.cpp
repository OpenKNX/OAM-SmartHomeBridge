#include <knx.h>
#include "Bridge.h"
#include "KnxBridgeDevice.h"

#define KO_WLAN_STATE KoBRI_WLANState, DPT_Switch

#include "KnxBridgeDevice.h"

KnxBridgeDevice::KnxBridgeDevice()
    : Component((const char *) ParamBRI_BridgeName)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin((const char*)ParamBRI_WiFiSSID, (const char*) ParamBRI_WiFiPassword);
}

void KnxBridgeDevice::initialize(std::list<IBridge *> *bridgeInterfaces)
{
    this->bridgeInterfaces = bridgeInterfaces;
    for (std::list<IBridge *>::iterator it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    (*it)->initialize(this);
}

void KnxBridgeDevice::loop(unsigned long now, bool initalize)
{       
    goSet(KO_WLAN_STATE, WiFi.status() == WL_CONNECTED, initalize);

    for (std::list<IBridge *>::iterator it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
        (*it)->loop();    
}

void KnxBridgeDevice::received(GroupObject& groupObject)
{
    for (std::list<IBridge *>::iterator it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
        (*it)->received(groupObject);
}

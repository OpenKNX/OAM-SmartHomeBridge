#include <knx.h>
#include "Bridge.h"
#include "KnxBridgeDevice.h"

#define GO_WLAN_STATE BRI_KoWLANState, "WLAN Connected", DPT_Switch

#include "KnxBridgeDevice.h"

KnxBridgeDevice::KnxBridgeDevice()
    : Component((const char *)deviceName, 0)
{
    readKnxParameterString("Bridge", "BridgeName", BRI_BridgeName, deviceName, sizeof(deviceName));
    mode = (Mode) readKnxParameterUInt8("Mode", BRI_Modus);
    readKnxParameterString("Bridge", "SSID", BRI_WiFiSSID, ssid, sizeof(ssid));
    readKnxParameterString("Bridge", "Password", BRI_WiFiPassword, password, sizeof(password));
    readKnxParameterString("Bridge", "PairingCode", BRI_PairingCode, pairingCode, sizeof(pairingCode));
 
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
}

void KnxBridgeDevice::initialize(std::list<IBridgeInterface *> *bridgeInterfaces)
{
    this->bridgeInterfaces = bridgeInterfaces;
    for (std::list<IBridgeInterface *>::iterator it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    (*it)->initialize(this);
}

void KnxBridgeDevice::loop(unsigned long now, bool initalize)
{       
    goSet(GO_WLAN_STATE, WiFi.status() == WL_CONNECTED, initalize);

    for (std::list<IBridgeInterface *>::iterator it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
        (*it)->loop();    
}

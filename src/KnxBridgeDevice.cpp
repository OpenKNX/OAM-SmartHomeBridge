#include <knx.h>
#include "Bridge.h"
#include "KnxBridgeDevice.h"

#define KO_WLAN_STATE KoBRI_WLANState, DPT_Switch

#include "KnxBridgeDevice.h"

KnxBridgeDevice::KnxBridgeDevice()
    : Component((const char *)deviceName)
{
    readKnxParameterString("Bridge", "BridgeName", ParamBRI_BridgeName, deviceName, sizeof(deviceName));
    mode = (Mode) readKnxParameterUInt8("Mode", ParamBRI_Modus);
    readKnxParameterString("Bridge", "SSID", ParamBRI_WiFiSSID, ssid, sizeof(ssid));
    readKnxParameterString("Bridge", "Password", ParamBRI_WiFiPassword, password, sizeof(password));
    readKnxParameterString("Bridge", "PairingCode", ParamBRI_PairingCode, pairingCode, sizeof(pairingCode));
 
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
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

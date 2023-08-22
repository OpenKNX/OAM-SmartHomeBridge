#include <knx.h>
#include "hardware.h"
#include <WiFi.h>
#include "SmartHomeBridgeModule.h"
#include "KnxChannelSwitch.h"
#include "KnxChannelDimmer.h"
#include "KnxChannelRolladen.h"
#include "KnxChannelJalousie.h"
#include "KnxChannelThermostat.h"
#include "KnxChannelDisplay.h"
#include "KnxChannelSensor.h"

#include "HomeKitBridge.h"
#include "HomeKitSwitch.h"
#include "HomeKitDimmer.h"
#include "HomeKitRolladen.h"
#include "HomeKitJalousie.h"
#include "HomeKitThermostat.h"
#include "HomeKitDisplay.h"
#include "HomeKitSensor.h"

#include "HueBridge.h"
#include "HueSwitch.h"
#include "HueDimmer.h"
#include "HueRolladen.h"
#include "HueJalousie.h"
#include "HueDimmer.h"
#include "SmartHomeBridgeModuleKnxProd.h"
#include "CP1252ToUTF8.h"

SmartHomeBridgeModule::SmartHomeBridgeModule()
  : ChannelOwnerModule(BRI_ChannelCount)
{
  
}


SmartHomeBridgeModule::~SmartHomeBridgeModule()
{
    if (_utf8Name != nullptr)
    {
        delete _utf8Name;
        _utf8Name = nullptr;
    }
}

const char* SmartHomeBridgeModule::getNameInUTF8()
{
    return _utf8Name;
}

void SmartHomeBridgeModule::setup()
{
    logDebugP("Setup Bridge");
    GroupObject& wlanState = KoBRI_WLANState;
    wlanState.value(false, DPT_Switch);
    _utf8Name = convert1252ToUTF8((const char*)ParamBRI_BridgeName);
    WiFi.mode(WIFI_STA);
    WiFi.begin((const char*)ParamBRI_WiFiSSID, (const char*) ParamBRI_WiFiPassword);

    Mode mode = (Mode) ParamBRI_Modus;
   
    bridgeInterfaces = new std::list<BridgeBase *>();
    if (mode & Mode::Homekit)
      bridgeInterfaces->push_back(new HomeKitBridge());

    if (mode & Mode::HueBridgeEmulation)
    {
      _pHueBridge = new HueBridge();
      bridgeInterfaces->push_back(_pHueBridge);
    }

    for (std::list<BridgeBase *>::iterator it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      (*it)->initialize(this);
    
    ChannelOwnerModule::setup();
  
}

OpenKNX::Channel* SmartHomeBridgeModule::createChannel(uint8_t _channelIndex /* this parameter is used in macros, do not rename */)
{
  Mode mode = (Mode) ParamBRI_Modus;
  int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
  uint8_t deviceType = ParamBRI_CHDeviceType;

  switch (deviceType)
  {
    case 0:
    {
      logInfoP("Device: %d AID: %d - Inactive", _channelIndex + 1, homekitAID);
      return nullptr;
    }
    case 1:
    {
      logInfoP("Device: %d AID: %d - Switch", _channelIndex + 1, homekitAID);
      std::list<SwitchBridge *> *switchBridges = new std::list<SwitchBridge *>();
      if (mode & Mode::Homekit)
        switchBridges->push_back(new HomeKitSwitch(homekitAID));
      if (mode & Mode::HueBridgeEmulation)
        switchBridges->push_back(new HueSwitch(_pHueBridge));
      return new KnxChannelSwitch(switchBridges, _channelIndex);
    }
    case 2:
    {
      logInfoP("Device: %d AID: %d - Dimmer", _channelIndex + 1, homekitAID);
      std::list<DimmerBridge *> *dimmerBridges = new std::list<DimmerBridge *>();
      if (mode & Mode::Homekit)
        dimmerBridges->push_back(new HomeKitDimmer(homekitAID));
      if (mode & Mode::HueBridgeEmulation)
        dimmerBridges->push_back(new HueDimmer(_pHueBridge));
      return new KnxChannelDimmer(dimmerBridges, _channelIndex);
    }
    case 3:
    {
      logInfoP("Device: %d AID: %d - Jalousien", _channelIndex + 1, homekitAID);
      std::list<RolladenBridge *> *jalousieBridges = new std::list<RolladenBridge *>();
      if (mode & Mode::Homekit)
        jalousieBridges->push_back(new HomeKitJalousie(homekitAID));
      if (mode & Mode::HueBridgeEmulation && BRI_CHJalousieHueEmulation)
        jalousieBridges->push_back(new HueJalousie(_pHueBridge));
      return new KnxChannelJalousie(jalousieBridges, _channelIndex);
    }
    case 4:
    {
      logInfoP("Device: %d AID: %d - Rolladen", _channelIndex + 1, homekitAID);
      std::list<RolladenBridge *> *rolladenBridges = new std::list<RolladenBridge *>();
      if (mode & Mode::Homekit)
        rolladenBridges->push_back(new HomeKitRolladen(homekitAID));
      if (mode & Mode::HueBridgeEmulation && BRI_CHRolladenHueEmulation)
        rolladenBridges->push_back(new HueRolladen(_pHueBridge));
      return new KnxChannelRolladen(rolladenBridges, _channelIndex);
    }
    case 5:
    {
      logInfoP("Device: %d AID: %d - Thermostat", _channelIndex + 1, homekitAID);
      std::list<ThermostatBridge *> *thermostatBridges = new std::list<ThermostatBridge *>();
      if (mode & Mode::Homekit)
        thermostatBridges->push_back(new HomeKitThermostat(homekitAID));   
      return new KnxChannelThermostat(thermostatBridges, _channelIndex);
    }
    case 6:
    {
      logInfoP("Device: %d AID: %d - Display", _channelIndex + 1, homekitAID);
      std::list<DisplayBridge *> *displayBridges = new std::list<DisplayBridge *>();
      if (mode & Mode::Homekit)
        displayBridges->push_back(new HomeKitDisplay(homekitAID));   
      return new KnxChannelDisplay(displayBridges, _channelIndex);
    }
    case 7:
    {
      logInfoP("Device: %d AID: %d - Sensor", _channelIndex + 1, homekitAID);
      std::list<SensorBridge *> *sensorBridges = new std::list<SensorBridge *>();
      if (mode & Mode::Homekit)
        sensorBridges->push_back(new HomeKitSensor(homekitAID));   
      return new KnxChannelSensor(sensorBridges, _channelIndex);
    }
    default:
    {
      logInfoP("Device: %d AID: %d - Unkown device type %d", _channelIndex + 1, homekitAID, deviceType);
      return nullptr;
    }
  }    
}


void SmartHomeBridgeModule::loop()
{
  bool connected = WiFi.status() == WL_CONNECTED;
  GroupObject& wlanState = KoBRI_WLANState;
  if (connected != (bool) wlanState.value(DPT_Switch))
      wlanState.value(connected, DPT_Switch);

  for (std::list<BridgeBase *>::iterator it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      (*it)->loop();    

  ChannelOwnerModule::loop();
}

void SmartHomeBridgeModule::processInputKo(GroupObject &ko)
{
    if (bridgeInterfaces != nullptr)
    {
      for (std::list<BridgeBase *>::iterator it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
          (*it)->processInputKo(ko);
    }
    ChannelOwnerModule::processInputKo(ko);
}
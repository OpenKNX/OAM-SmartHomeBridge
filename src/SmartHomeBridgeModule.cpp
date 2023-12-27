#include <Arduino.h>
#include <knx.h>
#include "hardware.h"
#include <WiFi.h>
#include "HomeSpan.h"
#include "SmartHomeBridgeModule.h"
#include "./Switch/KnxChannelSwitch.h"
#include "./Dimmer/KnxChannelDimmer.h"
#include "./Rolladen/KnxChannelRolladen.h"
#include "./Jalousie/KnxChannelJalousie.h"
#include "./Thermostat/KnxChannelThermostat.h"
#include "./Display/KnxChannelDisplay.h"
#include "./Sensor/KnxChannelSensor.h"
#include "./Fan/KnxChannelFan.h"
#include "./DoorWindow/KnxChannelDoorWindow.h"

#include "HomeKitBridge.h"
#include "./Switch/HomeKitSwitch.h"
#include "./Dimmer/HomeKitDimmer.h"
#include "./Rolladen/HomeKitRolladen.h"
#include "./Jalousie/HomeKitJalousie.h"
#include "./Thermostat/HomeKitThermostat.h"
#include "./Display/HomeKitDisplay.h"
#include "./Sensor/HomeKitSensor.h"
#include "./Fan/HomeKitFan.h"
#include "./DoorWindow/HomeKitDoorWindow.h"

#include "HueBridge.h"
#include "./Switch/HueSwitch.h"
#include "./Dimmer/HueDimmer.h"
#include "./Rolladen/HueRolladen.h"
#include "./Jalousie/HueJalousie.h"
#include "./Fan/HueFan.h"
#include "./DoorWindow/HueDoorWindow.h"

#include "knxprod.h"
#include "CP1252ToUTF8.h"

SmartHomeBridgeModule::SmartHomeBridgeModule()
  : ChannelOwnerModule(BRI_ChannelCount)
{
  
}

const std::string SmartHomeBridgeModule::name()
{
  return "SmartHomeBridgeModule";
}
        
const std::string SmartHomeBridgeModule::version()
{
  return "0.7";
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
    webServer = new WebServer(80);
    GroupObject& wlanState = KoBRI_WLANState;
    wlanState.value(false, DPT_Switch);
    _utf8Name = convert1252ToUTF8((const char*)ParamBRI_BridgeName);
    WiFi.mode(WIFI_STA);
    WiFi.begin((const char*)ParamBRI_WiFiSSID, (const char*) ParamBRI_WiFiPassword);

    Mode mode = (Mode) ParamBRI_Modus;
    if (mode & Mode::Homekit)
      logDebugP("Homekit enabled");
    if (mode & Mode::HueBridgeEmulation && BRI_CHRolladenHueEmulation)
      logDebugP("Hue enabled");

    bridgeInterfaces = new DynamicPointerArray<BridgeBase>();
    if (mode & Mode::Homekit)
      bridgeInterfaces->push_back(new HomeKitBridge());

    if (mode & Mode::HueBridgeEmulation)
    {
      _pHueBridge = new HueBridge();
      bridgeInterfaces->push_back(_pHueBridge);
    }

    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      (*it)->initialize(this);
    
    ChannelOwnerModule::setup();
    
 
}

OpenKNX::Channel* SmartHomeBridgeModule::createChannel(uint8_t _channelIndex /* this parameter is used in macros, do not rename */)
{

  Mode mode = (Mode) ParamBRI_Modus;
  int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
  uint8_t deviceType = ParamBRI_CHDeviceType;
  if (ParamBRI_CHDisableChannel && deviceType != 0)
  {
    logInfoP("Device: %d - Disabled", _channelIndex + 1);
    return nullptr;
  }
  switch (deviceType)
  {
    case 0:
    {
      logInfoP("Device: %d AID: %d - Inactive", _channelIndex + 1, homekitAID);
      return nullptr;
    }
    case 20:
    {
      logInfoP("Device: %d AID: %d - On/Off Light", _channelIndex + 1, homekitAID);
      auto switchBridges = new DynamicPointerArray<SwitchBridge>();
      if (mode & Mode::Homekit)
        switchBridges->push_back(new HomeKitSwitch(homekitAID));
      if (mode & Mode::HueBridgeEmulation && BRI_CHLightHueEmulation)
        switchBridges->push_back(new HueSwitch(_pHueBridge));
      return new KnxChannelSwitch(switchBridges, _channelIndex);
    }
    case 21:
    {
      logInfoP("Device: %d AID: %d - Dimmer", _channelIndex + 1, homekitAID);
      auto dimmerBridges = new DynamicPointerArray<DimmerBridge>();
      if (mode & Mode::Homekit)
        dimmerBridges->push_back(new HomeKitDimmer(homekitAID));
      if (mode & Mode::HueBridgeEmulation && BRI_CHLightHueEmulation)
        dimmerBridges->push_back(new HueDimmer(_pHueBridge));
      return new KnxChannelDimmer(dimmerBridges, _channelIndex);
    }
    case 30:
    {
      logInfoP("Device: %d AID: %d - Jalousien", _channelIndex + 1, homekitAID);
      auto jalousieBridges = new DynamicPointerArray<RolladenBridge>();
      if (mode & Mode::Homekit)
        jalousieBridges->push_back(new HomeKitJalousie(homekitAID));
      if (mode & Mode::HueBridgeEmulation && BRI_CHJalousieHueEmulation)
        jalousieBridges->push_back(new HueJalousie(_pHueBridge));
      return new KnxChannelJalousie(jalousieBridges, _channelIndex);
    }
    case 31:
    case 32:
    {
      logInfoP("Device: %d AID: %d - Rolladen", _channelIndex + 1, homekitAID);
      auto rolladenBridges = new DynamicPointerArray<RolladenBridge>();
      if (mode & Mode::Homekit)
        rolladenBridges->push_back(new HomeKitRolladen(homekitAID));
      if (mode & Mode::HueBridgeEmulation && BRI_CHRolladenHueEmulation)
        rolladenBridges->push_back(new HueRolladen(_pHueBridge));
      return new KnxChannelRolladen(rolladenBridges, _channelIndex);
    }
    case 50:
    {
      logInfoP("Device: %d AID: %d - Thermostat", _channelIndex + 1, homekitAID);
      auto thermostatBridges = new DynamicPointerArray<ThermostatBridge>();
      if (mode & Mode::Homekit)
        thermostatBridges->push_back(new HomeKitThermostat(homekitAID));   
      return new KnxChannelThermostat(thermostatBridges, _channelIndex);
    }
    case 60:
    case 61:
    case 62:
    {
      logInfoP("Device: %d AID: %d - Display", _channelIndex + 1, homekitAID);
      auto displayBridges = new DynamicPointerArray<DisplayBridge>();
      if (mode & Mode::Homekit)
        displayBridges->push_back(new HomeKitDisplay(homekitAID));   
      return new KnxChannelDisplay(displayBridges, _channelIndex);
    }
    case 70:
    case 71:
    case 72:
    case 73:
    case 74:
    case 75:
    case 76:
    {
      logInfoP("Device: %d AID: %d - Sensor", _channelIndex + 1, homekitAID);
      auto sensorBridges = new DynamicPointerArray<SensorBridge>();
      if (mode & Mode::Homekit)
        sensorBridges->push_back(new HomeKitSensor(homekitAID));   
      return new KnxChannelSensor(sensorBridges, _channelIndex);
    }
    case 80:
    {
      logInfoP("Device: %d AID: %d - Fan", _channelIndex + 1, homekitAID);
      auto fanBridges = new DynamicPointerArray<FanBridge>();
      if (mode & Mode::Homekit)
        fanBridges->push_back(new HomeKitFan(homekitAID));
      if (mode & Mode::HueBridgeEmulation && BRI_CHFanHueEmulation)
        fanBridges->push_back(new HueFan(_pHueBridge));
      return new KnxChannelFan(fanBridges, _channelIndex);
    }
    case 90:
    case 91:
    case 92:
    {
      logInfoP("Device: %d AID: %d - DoorWindow", _channelIndex + 1, homekitAID);
      auto doorWindowBridges = new DynamicPointerArray<DoorWindowBridge>();
      if (mode & Mode::Homekit)
        doorWindowBridges->push_back(new HomeKitDoorWindow(homekitAID));
      if (mode & Mode::HueBridgeEmulation && BRI_CHFanHueEmulation)
        doorWindowBridges->push_back(new HueDoorWindow(_pHueBridge));
      return new KnxChannelDoorWindow(doorWindowBridges, _channelIndex);
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
  if (connected && !started)
  {
    started = true;
    webServer = new WebServer(80);
    // serve pages
    webServer->on("/", HTTP_GET, [=](){serveHomePage();});
    webServer->on("/updateFW", HTTP_GET, [=](){serveFirmwareUpdatePage();});
    webServer->on("/reboot", HTTP_POST, [=](){serveRebootPage();});
    // handling uploading firmware file
    webServer->on("/update", HTTP_POST, [=]() {
      webServer->sendHeader("Connection", "close");
      webServer->send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, [=]() {
      HTTPUpload& upload = webServer->upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
      }
    });

    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      (*it)->initWebServer(*webServer);

    webServer->enableDelay(false);
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      (*it)->start(this);
    webServer->begin();
  }
  webServer->handleClient();

  GroupObject& wlanState = KoBRI_WLANState;
  if (connected != (bool) wlanState.value(DPT_Switch))
      wlanState.value(connected, DPT_Switch);


  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      (*it)->loop();    

  ChannelOwnerModule::loop();
}

#ifdef OPENKNX_DUALCORE
void SmartHomeBridgeModule::loop1()
{
  for (std::vector<BridgeBase *>::iterator it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      (*it)->loop1();    

  ChannelOwnerModule::loop1();
}
#endif

void SmartHomeBridgeModule::processInputKo(GroupObject &ko)
{
    if (bridgeInterfaces != nullptr)
    {
      for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
          (*it)->processInputKo(ko);
    }
    ChannelOwnerModule::processInputKo(ko);
}

WebServer* SmartHomeBridgeModule::getWebServer()
{
    return webServer;
}


const char* firmwareUpdatePage =
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

void SmartHomeBridgeModule::serveFirmwareUpdatePage()
{
   webServer->send(200, "text/html;charset=UTF-8", firmwareUpdatePage);

}


void SmartHomeBridgeModule::serveRebootPage()
{
    String res = "<!DOCTYPE html><html lang=\"en\"><meta charset=\"UTF-8\"><meta http-equiv=\"refresh\" content=\"20;url=/\"><title>";
    res + "Smart Home Bridge Reboot";
    res += "</title><body>";
    res += "<br>Smart Home Bridge is rebooting...</br>";
    res += "</body>";
    webServer->send(200, "text/html;charset=UTF-8", res);
    vTaskDelay(1000);
    ESP.restart();
}

void SmartHomeBridgeModule::serveHomePage()
{
  auto progMode = webServer->arg("progMode");
  if (progMode == "1")
    knx.progMode(true);
  else if (progMode == "0")
    knx.progMode(false);
  auto name = String(getNameInUTF8());
  name.replace("<", "&lt;");
  name.replace(">", "&gt;");
  name.replace("&", "&amp;");
  name.replace("\"", "&quot;");

  String res = "<!DOCTYPE html><html lang=\"en\"><meta charset=\"UTF-8\"><meta http-equiv=\"refresh\" content=\"10;url=/\"><title>";
  res += name;
  res += "</title><body>";
  res += "<h1>OpenKNX SmartHome Bridge</h1>";
  res += "© Copyright OpenKNX, Michael Geramb, ";
  res += (__DATE__ + sizeof(__DATE__) - 5);
  res += "<br><br>Name: ";
  res += name;
  res += "<br>ETS Gerätetype: 0x";
  char etsType[5];
  sprintf(etsType, "%02X%02X", MAIN_OpenKnxId, MAIN_ApplicationNumber);
  res += etsType;
  res += "<br>ETS App Version: ";
  res += MAIN_ApplicationVersion;
  res += "<br>KNX Address: ";
  res += openknx.info.humanIndividualAddress().c_str();
  res += "<br>KNX Version: ";
  res += KNX_Version;
  res += "<br>Version: ";
  res += MAIN_Version;
  res += "<br>Common Version: ";
  res += MODULE_Common_Version;
  res += "<br>Logic Modul Version: ";
  res += MODULE_LogicModule_Version;
  res += "<br>Arduino Version: ";
  res += ESP_ARDUINO_VERSION_MAJOR;
  res += ".";
  res += ESP_ARDUINO_VERSION_MINOR;
  res += ".";
  res += ESP_ARDUINO_VERSION_PATCH;
  res += "<br>Used channels: " + (String)getNumberOfUsedChannels();
  res += " of " + (String) BRI_ChannelCount;
  res += "<br>Free Heap: " + (String)ESP.getFreeHeap() + " of " + (String) ESP.getHeapSize();
  res += "<br>Min Heap: " + (String)ESP.getMinFreeHeap();
  res += "<br>Max Free Block: " + (String)ESP.getMaxAllocHeap();
  if (ESP.getFreePsram() > 0)
  {
    res += "<br>Free PSRAM: " + (String)ESP.getFreePsram() + " of " + (String) ESP.getPsramSize();
    res += "<br>Min PSRAM: " + (String)ESP.getMinFreePsram();
  }
  res += "<br>Max Stack Usage: " + (String) (8192 - uxTaskGetStackHighWaterMark(nullptr));
  res += " of 8192"; 
  res += "<br>Uptime: " + (String)millis();
  res += "<h2>Bridges:</h2>";
  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
  {
    (*it)->getInformation(res);    
    res += "<br>";
  }
  // prog button
  res += "<h2>Control</h2><form method='post' action='/'><input name='progMode' type='hidden' value='";
  res += knx.progMode() ?  "0" : "1";
  res += "'><input type='submit' value='";
  res += knx.progMode() ?  "Stop Programming Mode" : "Start Programming Mode";
  res += "'></form>";
  // reset button
  res += "<form method='post' action='/reboot'><input type='submit' value='Reboot'></form>";
  // firmware update button
  //res += "<form action='/updateFW'><button type='submit'>Update Firmware</button></form>";
  res += "</body>";
  webServer->send(200, "text/html;charset=UTF-8", res);

}

SmartHomeBridgeModule openknxSmartHomeBridgeModule;
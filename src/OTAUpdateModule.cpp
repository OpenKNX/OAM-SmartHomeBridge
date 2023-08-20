#include <ArduinoOTA.h>
#include "OTAUpdateModule.h"

const std::string OTAUpdateModule::name()
{
    return "OTAUpdateModule";
}

void OTAUpdateModule::setup()
{
    ArduinoOTA
        .onStart([&]() {
            if (ArduinoOTA.getCommand() == U_FLASH)
                logInfoP("Start updating firmware");
            else // U_SPIFFS
                logInfoP("Start updating filesystem");
        })
        .onEnd([&]() {
            logInfoP("End update");
        })
        .onProgress([&](unsigned int progress, unsigned int total) {
            logInfoP("Progress: %.1f%%", (float) progress / (total / 100.0));
        })
        .onError([&](ota_error_t error) {
            const char* errorText = "unkown";
            if (error == OTA_AUTH_ERROR) errorText = "auth failed";
            else if (error == OTA_BEGIN_ERROR) errorText = "begin failed";
            else if (error == OTA_CONNECT_ERROR) errorText = "connect failed";
            else if (error == OTA_RECEIVE_ERROR) errorText = "receive failed";
            else if (error == OTA_END_ERROR) errorText = "end failed";
            logErrorP("Error[%d]: %s", error, errorText);  
            });
    ArduinoOTA.begin();
    started = true;
}

void OTAUpdateModule::loop()
{
    if (started)
        ArduinoOTA.handle();
}
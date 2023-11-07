#include <ArduinoOTA.h>
#include "OTAUpdateModule.h"

const std::string OTAUpdateModule::name()
{
    return "OTAUpdateModule";
}

const std::string OTAUpdateModule::version()
{
    return "0";
}

static RTC_NOINIT_ATTR  bool rebootAfterSWUpdate=false;
void OTAUpdateModule::setup(bool configured)
{
    if (!configured)
    {
        if (rebootAfterSWUpdate)
        {
            // configuration not loaded after flash, active prog mode
            rebootAfterSWUpdate = false;
            knx.progMode(true);
        }
    }
    else
    {   
        if (rebootAfterSWUpdate)
            rebootAfterSWUpdate = false;
        ArduinoOTA
            .onStart([&]() {
                if (ArduinoOTA.getCommand() == U_FLASH)
                    logInfoP("Start updating firmware");
                else // U_SPIFFS
                    logInfoP("Start updating filesystem");
            })
            .onEnd([&]() {
                logInfoP("End update");
                rebootAfterSWUpdate = true;
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
}

void OTAUpdateModule::loop()
{
    ArduinoOTA.handle();
}
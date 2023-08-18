#include "Bridge.h"
#include "KnxChannelBase.h"
#include "CP1252ToUTF8.h"

KnxChannelBase::KnxChannelBase(uint16_t _channelIndex) // Do not rename the parameter name, it is used in the ParamBRI_CHDeviceName macro
    : Component((const char*) ParamBRI_CHDeviceName)
{
    this->_channelIndex = _channelIndex;
  //  readKnxParameterString("DeviceName", ParamBRI_CHDeviceName, deviceName, sizeof(this->deviceName));
    Serial.println(componentName);
      Serial.println(strlen(componentName));
    utf8Name = convert1252ToUTF8(componentName);
    Serial.println(utf8Name);
      Serial.println(strlen(utf8Name));
}

const char* KnxChannelBase::getNameInUTF8()
{
    return utf8Name;
}

KnxChannelBase::~KnxChannelBase()
{
    if (utf8Name != NULL)
    {
        delete utf8Name;
        utf8Name = NULL;
    }
}
  

const std::string KnxChannelBase::name()
{
    return componentName;
}
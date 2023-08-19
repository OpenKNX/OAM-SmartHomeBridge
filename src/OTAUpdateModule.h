#pragma once
#include "OpenKNX.h"

class OTAUpdateModule : public OpenKNX::Module
{
public:
    virtual const std::string name();
    virtual void setup();
    virtual void loop();

};
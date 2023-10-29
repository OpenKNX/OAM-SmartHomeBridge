#pragma once
#include "OpenKNX.h"

class OTAUpdateModule : public OpenKNX::Module
{
    bool started = false;
public:
    virtual const std::string name() override;
    virtual const std::string version() override;
    virtual void setup() override;
    virtual void loop() override;

};
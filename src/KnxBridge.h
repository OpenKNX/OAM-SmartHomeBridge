
#pragma once
#include "OpenKNX.h"
#include "Component.h"

class KnxBridge : public OpenKNX::Module
{
    private:
        std::list<Component*> _components;   
    protected:
        virtual void setup();
        virtual void processInputKo(GroupObject &ko);
};
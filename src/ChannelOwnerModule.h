#pragma once
#include "OpenKNX.h"

class ChannelOwnerModule : public OpenKNX::Module
{
private: 
    uint8_t _numberOfChannels;
    uint8_t _currentChannel = 0;
    OpenKNX::Channel** _pChannels = nullptr;
public:
    ChannelOwnerModule(uint8_t numberOfChannels = 0);
    ~ChannelOwnerModule();

    virtual OpenKNX::Channel* createChannel(uint8_t _channelIndex /* this parameter is used in macros, do not rename */); 

    /*
        * Called during startup after initialization of all modules is completed.
        * Useful for init interrupts on core0
        */
    virtual void setup(bool configured) override;
    virtual void setup() override;

    /*
        * Module logic for core0
        */
    virtual void loop(bool configured) override;
    virtual void loop() override;
    uint16_t getNumberOfUsedChannels();

#ifdef OPENKNX_DUALCORE
    /*
        * Called during startup after setup() completed
        * Useful for init interrupts on core1
        */
    virtual void setup1(bool configured) override;
    virtual void setup1() override;

    /*
        * Module logic for core1
        */
    virtual void loop1(bool configured) override;
    virtual void loop1() override;
#endif

#if (MASK_VERSION & 0x0900) != 0x0900 // Coupler do not have GroupObjects
    /*
        * Called on incoming/changing GroupObject
        * @param GroupObject
        */
    virtual void processInputKo(GroupObject &ko) override;
#endif
};
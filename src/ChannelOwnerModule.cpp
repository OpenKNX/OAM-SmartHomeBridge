#include "ChannelOwnerModule.h"

ChannelOwnerModule::ChannelOwnerModule(uint8_t numberOfChannels)
    : _numberOfChannels(numberOfChannels)
{
    if (_numberOfChannels > 0)
    {
        _pChannels = new OpenKNX::Channel*[numberOfChannels]();
    }
}

ChannelOwnerModule::~ChannelOwnerModule()
{
    if (_pChannels != NULL)
    {
        delete[] _pChannels;
        _pChannels = NULL;
    }
}

/*
    * Called during startup after initialization of all modules is completed.
    * Useful for init interrupts on core0
    */
void ChannelOwnerModule::setup(bool configured)
{
    OpenKNX::Module::setup(configured);
}

OpenKNX::Channel* ChannelOwnerModule::createChannel(uint8_t _channelIndex /* this parameter is used in macros, do not rename */)
{
    return NULL;
}

void ChannelOwnerModule::setup()
{
    OpenKNX::Module::setup();
    if (_pChannels != NULL)
    {
        logInfoP("Setting up %d channels", _numberOfChannels);
        for (uint8_t _channelIndex = 0; _channelIndex < _numberOfChannels; _channelIndex++)
        {
            logInfoP("Create channel %d", _channelIndex);  
            logIndentUp();
            _pChannels[_channelIndex] = createChannel(_channelIndex);
            logIndentDown();
        }
        for (uint8_t _channelIndex = 0; _channelIndex < _numberOfChannels; _channelIndex++)
        {
            OpenKNX::Channel* channel = _pChannels[_channelIndex];
            if (channel != NULL)
            {
                logInfoP("Init channel %d", _channelIndex);  
                logIndentUp();
                channel->init();
                logIndentDown();

                logInfoP("Setup channel %d - setup(true)", _channelIndex);  
                logIndentUp();
                channel->setup(true);
                logIndentDown();

                logInfoP("Setup channel %d - setup()", _channelIndex);  
                logIndentUp();
                channel->setup();
                logIndentDown();
            }

        }
    }
}

/*
    * Module logic for core0
    */
void ChannelOwnerModule::loop(bool configured)
{
    OpenKNX::Module::loop(configured);
    if (_pChannels != NULL)
    {
        for (uint8_t _channelIndex= 0; _channelIndex < _numberOfChannels; _channelIndex++)
        {
            OpenKNX::Channel* channel = _pChannels[_channelIndex] ;
            if (channel != NULL)
            {
                channel->loop(configured);
            }
        }
    }
}

void ChannelOwnerModule::loop()
{
    OpenKNX::Module::loop();
    if (_pChannels != NULL)
    {
        for (uint8_t _channelIndex = 0; _channelIndex < _numberOfChannels; _channelIndex++)
        {
            OpenKNX::Channel* channel = _pChannels[_channelIndex] ;
            if (channel != NULL)
            {
                channel->loop();
            }
        }
    }
}

#ifdef OPENKNX_DUALCORE
/*
    * Called during startup after setup() completed
    * Useful for init interrupts on core1
    */
void ChannelOwnerModule::ChannelOwnerModule::setup1(bool configured)
{
    OpenKNX::Module::setup1(configured);
    if (_pChannels != NULL)
    {
        for (uint8_t _channelIndex = 0; _channelIndex < _numberOfChannels; _channelIndex++)
        {
            OpenKNX::Channel* channel = _pChannels[_channelIndex];
            if (channel != NULL)
            {
                channel->setup1(configured);
            }
        }
    }
}
void ChannelOwnerModule::ChannelOwnerModule::setup1()
{
    OpenKNX::Module::setup1();
    if (_pChannels != NULL)
    {
        for (uint8_t _channelIndex = 0; _channelIndex < _numberOfChannels; _channelIndex++)
        {
            OpenKNX::Channel* channel = _pChannels[_channelIndex];
            if (channel != NULL)
            {
                channel->setup1();
            }
        }
    }
}
/*
    * Module logic for core1
    */
void ChannelOwnerModule::loop1(bool configured)
{
    OpenKNX::Module::loop1(configured);
    if (_pChannels != NULL)
    {
        for (uint8_t _channelIndex = 0; _channelIndex < _numberOfChannels; _channelIndex++)
        {
            OpenKNX::Channel* channel = _pChannels[_channelIndex];
            if (channel != NULL)
            {
                channel->loop1(configured);
            }
        }
    }
}
void ChannelOwnerModule::loop1()
{
    OpenKNX::Module::loop1();
    if (_pChannels != NULL)
    {
        for (uint8_t _channelIndex = 0; _channelIndex < _numberOfChannels; _channelIndex++)
        {
            OpenKNX::Channel* channel = _pChannels[_channelIndex];
            if (channel != NULL)
            {
                channel->loop1();
            }
        }
    }
}
#endif

#if (MASK_VERSION & 0x0900) != 0x0900 // Coupler do not have GroupObjects
/*
    * Called on incoming/changing GroupObject
    * @param GroupObject
    */
void ChannelOwnerModule::processInputKo(GroupObject &ko)
{
    OpenKNX::Module::processInputKo(ko);
    if (_pChannels != NULL)
    {
        for (uint8_t _channelIndex = 0; _channelIndex < _numberOfChannels; _channelIndex++)
        {
            OpenKNX::Channel* channel = _pChannels[_channelIndex];
            if (channel != NULL)
            {
                channel->processInputKo(ko);
            }
        }
    }
}
#endif
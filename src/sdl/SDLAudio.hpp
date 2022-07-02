#pragma once

#include <SDL.h>

#include "../audio/Device.hpp"

const uint8_t DEVICE_NUM_CHANNELS = 1;
const uint32_t DEVICE_SAMPLE_FREQ = 44100;

namespace Application
{
namespace SDL
{

class SDLAudio : public Audio::Device
{
public:
    SDLAudio();
    virtual ~SDLAudio();

    void InitComplete() override;

private:
    SDL_AudioDeviceID audio_device;
    SDL_AudioSpec audio_spec;
};
}
}

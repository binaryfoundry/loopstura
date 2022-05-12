#pragma once

#include <SDL.h>

#include "../Audio.hpp"

class SDLAudio : public Audio
{
public:
    SDLAudio();
    virtual ~SDLAudio();

    void InitComplete();

private:
    SDL_AudioDeviceID audio_device;
    SDL_AudioSpec audio_spec;

protected:
    void Queue(const void* data, uint32_t len);
};

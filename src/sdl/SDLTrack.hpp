#pragma once

#include <SDL.h>

#include "../audio/Track.hpp"

class SDLTrack : public Track
{
public:
    SDLTrack();
    virtual ~SDLTrack();

    void InitComplete();

private:
    SDL_AudioDeviceID audio_device;
    SDL_AudioSpec audio_spec;

protected:
    void Queue(const void* data, uint32_t len);
};

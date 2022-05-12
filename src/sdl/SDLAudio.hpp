#pragma once

#include <SDL.h>
#include <thread>
#include <memory>

#include "../RingBuffer.hpp"

class SDLAudio
{
public:
    SDLAudio();
    virtual ~SDLAudio();

private:
    SDL_AudioDeviceID audio_device;
    SDL_AudioSpec audio_spec;

    void Update();

    std::atomic<bool> input_thread_running;
    std::unique_ptr<std::thread> input_thread;

    float x = 0;
};

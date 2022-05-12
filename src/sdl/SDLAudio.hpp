#pragma once

#include <SDL.h>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "../RingBuffer.hpp"

#define INPUT_BUFFER_SIZE 1024
#define SAMPLE_FREQ 44100

class SDLAudio
{
public:
    SDLAudio();
    virtual ~SDLAudio();

private:
    SDL_AudioDeviceID audio_device;
    SDL_AudioSpec audio_spec;

    void ReadInput();

    std::atomic<bool> input_thread_running;
    std::unique_ptr<std::thread> input_thread;

    std::mutex input_mutex;
    std::condition_variable input_cond_var;

    RingBuffer<int16_t, INPUT_BUFFER_SIZE * 16> input_buffer;

    void WriteOutput();

    std::atomic<bool> output_thread_running;
    std::unique_ptr<std::thread> output_thread;

    float x = 0;
};

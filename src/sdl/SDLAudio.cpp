#include "SDLAudio.hpp"

#include <iostream>

namespace Application
{
namespace SDL
{

void audio_callback(void* user_data, Uint8* raw_buffer, int bytes)
{
    uint16_t* buffer = (uint16_t*)raw_buffer;
    int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
    SDLAudio* obj = (SDLAudio*)user_data;

    obj->Generate(buffer, length);
}

SDLAudio::SDLAudio()
{
    int count = SDL_GetNumAudioDevices(0);
    for (int i = 0; i < count; ++i)
    {
        std::cout << "Device " << i << ": " << SDL_GetAudioDeviceName(i, 0) << std::endl;
    }

    SDL_zero(audio_spec);
    audio_spec.freq = DEVICE_SAMPLE_FREQ;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = DEVICE_NUM_CHANNELS;
    audio_spec.samples = 2048;
    audio_spec.callback = audio_callback;
    audio_spec.userdata = this;

    audio_device = SDL_OpenAudioDevice(
        NULL,
        0,
        &audio_spec,
        NULL,
        0);
}

SDLAudio::~SDLAudio()
{
    SDL_CloseAudioDevice(
        audio_device);
}

void SDLAudio::InitComplete()
{
    SDL_PauseAudioDevice(
        audio_device,
        0);
}

}
}

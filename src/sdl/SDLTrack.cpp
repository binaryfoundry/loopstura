#include "SDLTrack.hpp"

void audio_callback(void* user_data, Uint8* raw_buffer, int bytes)
{
    uint16_t* buffer = (uint16_t*)raw_buffer;
    int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
    SDLTrack* obj = (SDLTrack*)user_data;

    obj->Generate(buffer, length);
}

SDLTrack::SDLTrack()
{
    SDL_zero(audio_spec);
    audio_spec.freq = SAMPLE_FREQ;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = NUM_CHANNELS;
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

SDLTrack::~SDLTrack()
{
    SDL_CloseAudioDevice(
        audio_device);
}

void SDLTrack::InitComplete()
{
    SDL_PauseAudioDevice(
        audio_device,
        0);
}

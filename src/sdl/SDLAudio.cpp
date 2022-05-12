#include "SDLAudio.hpp"

SDLAudio::SDLAudio()
{
    SDL_zero(audio_spec);
    audio_spec.freq = SAMPLE_FREQ;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 1;
    audio_spec.samples = INPUT_BUFFER_SIZE;
    audio_spec.callback = NULL;

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

void SDLAudio::Queue(const void* data, uint32_t len)
{
    SDL_QueueAudio(
        audio_device,
        data,
        len);
}

#include "SDLAudio.hpp"

#include <chrono>
#include <ratio>

template<std::intmax_t N, std::intmax_t D>
class UpdateLimiter {
public:
    UpdateLimiter() :
        time_between_frames{ 1 },
        tp{ std::chrono::steady_clock::now() }
    {
    }

    void sleep() {
        tp += time_between_frames;
        std::this_thread::sleep_until(tp);
    }

private:
    std::chrono::duration<double, std::ratio<N, D>> time_between_frames;
    std::chrono::time_point<std::chrono::steady_clock, decltype(time_between_frames)> tp;
};

SDLAudio::SDLAudio()
{
    SDL_zero(audio_spec);
    audio_spec.freq = 44100;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 1;
    audio_spec.samples = 1024;
    audio_spec.callback = NULL;

    audio_device = SDL_OpenAudioDevice(
        NULL,
        0,
        &audio_spec,
        NULL,
        0);

    input_thread_running = true;

    input_thread = std::make_unique<std::thread>([&]
    {
        UpdateLimiter<1024, 44100> limiter;
        while (input_thread_running)
        {
            Update();
            limiter.sleep();
        }
    });

    SDL_PauseAudioDevice(
        audio_device,
        0);
}

SDLAudio::~SDLAudio()
{
    input_thread_running = false;
    input_thread->join();

    SDL_CloseAudioDevice(
        audio_device);
}

void SDLAudio::Update()
{
    for (int i = 0; i < audio_spec.samples; i++)
    {
        x += .010f;

        // SDL_QueueAudio expects a signed 16-bit value
        // note: "5000" here is just gain so that we will hear something
        int16_t sample = sin(x * 4) * 5000;

        const int sample_size = sizeof(int16_t) * 1;
        SDL_QueueAudio(
            audio_device,
            &sample,
            sample_size);
    }
}

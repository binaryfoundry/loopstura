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

    for (uint32_t i = 0; i < 8; i++)
    {
        ReadInput();
    }

    input_thread_running = true;

    input_thread = std::make_unique<std::thread>([&]
    {
        UpdateLimiter<INPUT_BUFFER_SIZE, SAMPLE_FREQ> limiter;
        while (input_thread_running)
        {
            ReadInput();
            input_cond_var.notify_one();
            limiter.sleep();
        }
    });

    output_thread_running = true;

    output_thread = std::make_unique<std::thread>([&]
    {
        while (output_thread_running)
        {
            std::unique_lock<std::mutex> lk(input_mutex);

            input_cond_var.wait(lk, [&] {
                return !input_buffer.Empty();
            });

            WriteOutput();
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

    output_thread_running = false;
    output_thread->join();

    SDL_CloseAudioDevice(
        audio_device);
}

void SDLAudio::ReadInput()
{
    for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
    {
        x += .010f;
        int16_t sample = sin(x * 3.142) * 5000;
        input_buffer.Write(sample);
    }
}

void SDLAudio::WriteOutput()
{
    while (!input_buffer.Empty())
    {
        int16_t sample = input_buffer.Read();

        SDL_QueueAudio(
            audio_device,
            &sample,
            sizeof(uint16_t));
    }
}

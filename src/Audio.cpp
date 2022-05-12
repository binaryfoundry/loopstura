#include "Audio.hpp"

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

Audio::Audio()
{
    wav_file = std::make_unique<WAVFile>("D:\\file_example_WAV_10MG.wav");

    for (uint32_t i = 0; i < 8; i++)
    {
        ReadInput();
    }

    input_thread = std::make_unique<std::thread>([&]
    {
        UpdateLimiter<INPUT_BUFFER_SIZE, SAMPLE_FREQ> limiter;
        while (true)
        {
            ReadInput();
            input_cond_var.notify_one();
            limiter.sleep();
        }
    });

    input_thread->detach();

    output_thread = std::make_unique<std::thread>([&]
    {
        while (true)
        {
            std::unique_lock<std::mutex> lk(input_mutex);

            input_cond_var.wait(lk, [&] {
                return !input_buffer.Empty();
            });

            WriteOutput();
        }
    });

    output_thread->detach();
}

Audio::~Audio()
{

}

void Audio::ReadInput()
{
    for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
    {
        int16_t sample = wav_file->ReadSample<int16_t>();
        input_buffer.Write(sample);

        // TODO account for # of samples
        wav_file->ReadSample<int16_t>();
    }
}

void Audio::WriteOutput()
{
    while (!input_buffer.Empty())
    {
        int16_t sample = input_buffer.Read();

        Queue(
            &sample,
            sizeof(uint16_t));
    }
}
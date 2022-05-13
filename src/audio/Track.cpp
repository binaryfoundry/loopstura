#include "Track.hpp"

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

Track::Track()
{
    wav_file = std::make_unique<WAVFile>("D:\\file_example_WAV_10MG.wav");

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
                return !input_buffer.Empty() || (!output_thread_running);
            });

            if (output_thread_running)
            {
                WriteOutput();
            }
        }
    });
}

Track::~Track()
{
    input_thread_running = false;
    input_thread->join();

    output_thread_running = false;
    input_cond_var.notify_one();
    output_thread->join();
}

void Track::ReadInput()
{
    for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
    {
        const int16_t sample = wav_file->ReadSample<int16_t>();
        input_buffer.Write(sample);

        // TODO account for # of samples
        wav_file->ReadSample<int16_t>();
    }
}

void Track::WriteOutput()
{
    while (!input_buffer.Empty())
    {
        const int16_t sample = input_buffer.Read();

        Queue(
            &sample,
            sizeof(uint16_t));
    }
}

#include "Track.hpp"

#include <chrono>
#include <ratio>

#include "../signal/FFT.hpp"

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
    paused = true;

    wav_file = std::make_unique<WAVFile>("D:\\file_example_WAV_10MG.wav");

    waveform = std::make_unique<Waveform>(512);

    input_thread_running = true;

    input_thread = std::make_unique<std::thread>([&]
    {
        UpdateLimiter<WINDOW_SIZE, SAMPLE_FREQ> limiter;
        while (input_thread_running)
        {
            if (!paused)
            {
                for (int i = 0; i < WINDOW_SIZE; i++)
                {
                    const int16_t sample = wav_file->ReadSample<int16_t>();
                    input_buffer.Write(sample);

                    wav_file->ReadSample<int16_t>();
                    wav_file->position += speed_scale;
                }
            }

            input_cond_var.notify_one();
            limiter.sleep();
        }
    });

    output_thread_running = true;

    output_thread = std::make_unique<std::thread>([&]
    {
        std::array<std::complex<double>, WINDOW_SIZE> fft_buf;

        while (output_thread_running)
        {
            std::unique_lock<std::mutex> lk(input_mutex);

            input_cond_var.wait(lk, [&] {
                return !input_buffer.Empty() || !output_thread_running;
            });

            if (output_thread_running)
            {
                while (!input_buffer.Empty())
                {
                    const float in = static_cast<float>(input_buffer.Read());

                    processingInputBuffer[processingInputBufferPointer++] = in;
                    if (processingInputBufferPointer >= PROCESSING_BUFFER_SIZE)
                    {
                        // Wrap the circular buffer
                        // Notice: this is not the condition for starting a new FFT
                        processingInputBufferPointer = 0;
                    }

                    //get the output sample from the output buffer
                    float out = processing_output_buffer[processing_output_buffer_read_pointer];

                    // then clear the output sample in the buffer so it is ready for the next overlap-add
                    processing_output_buffer[processing_output_buffer_read_pointer] = 0;

                    // Scale the output down by the overlap factor (e.g. how many windows overlap per sample?)
                    out *= static_cast<float>(HOP_SIZE) / WINDOW_SIZE;

                    // increment the read pointer in the output cicular buffer
                    processing_output_buffer_read_pointer++;
                    if (processing_output_buffer_read_pointer >= PROCESSING_BUFFER_SIZE)
                    {
                        processing_output_buffer_read_pointer = 0;
                    }

                    // increment the hop counter and start a new FFT if we've reached the hop size
                    if (++hop_counter >= HOP_SIZE)
                    {
                        hop_counter = 0;

                        process_fft(
                            processingInputBuffer, processingInputBufferPointer,
                            processing_output_buffer, processing_output_buffer_write_pointer,
                            fft_buf);

                        processing_output_buffer_write_pointer =
                            (processing_output_buffer_write_pointer + HOP_SIZE) % PROCESSING_BUFFER_SIZE;
                    }

                    const uint16_t sample = static_cast<uint16_t>(out);

                    Queue(
                        &sample,
                        sizeof(uint16_t));
                }
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

void Track::SetPaused(bool value)
{
    paused = value;
}

void Track::DrawWaveform(const double scale)
{
    wav_file->Draw(waveform, scale);
}

void process_fft(
    std::array<float, PROCESSING_BUFFER_SIZE> const& in_buffer, unsigned int in_pointer,
    std::array<float, PROCESSING_BUFFER_SIZE>& out_buffer, unsigned int out_pointer,
    std::array<std::complex<double>, WINDOW_SIZE>& fft_buf)
{
    // Copy buffer into FFT input, starting one window ago
    for (int n = 0; n < WINDOW_SIZE; n++)
    {
        // Use modulo arithmetic to calculate the circular buffer index
        const size_t circular_buffer_index = (in_pointer + n - WINDOW_SIZE + PROCESSING_BUFFER_SIZE) % PROCESSING_BUFFER_SIZE;
        fft_buf[n] = std::complex<double>(in_buffer[circular_buffer_index]);
    }

    // Process the FFT based on the time domain input
    fft(fft_buf);

    // Robotise the output
    for(int n = 0; n < WINDOW_SIZE; n++)
    {
        const Complex v = fft_buf[n];
        double amplitude = std::sqrt(v.real() * v.real() + v.imag() * v.imag());
        fft_buf[n] = std::complex<double>(amplitude, 0);
    }

    // Run the inverse FFT
    ifft(fft_buf);

    // Add timeDomainOut into the output buffer starting at the write pointer
    for (int n = 0; n < WINDOW_SIZE; n++)
    {
        const size_t circular_buffer_index = (out_pointer + n) % PROCESSING_BUFFER_SIZE;
        out_buffer[circular_buffer_index] += static_cast<float>(fft_buf[n].real());
    }
}
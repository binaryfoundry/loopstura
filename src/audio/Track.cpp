#include "Track.hpp"

#include <chrono>
#include <ratio>
#include <iostream>

#include "../signal/Windowing.hpp"

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

std::array<double, WINDOW_SIZE> hann_window;

static std::array<double, WINDOW_SIZE / 2 + 1> analysisMagnitudes;
static std::array<double, WINDOW_SIZE / 2 + 1> analysisFrequencies;
static std::array<double, WINDOW_SIZE> last_input_phases;

double wrap_phase(double phaseIn)
{
    if (phaseIn >= 0)
        return fmod(phaseIn + std::numbers::pi, 2.0 * std::numbers::pi) - std::numbers::pi;
    else
        return fmod(phaseIn - std::numbers::pi, -2.0 * std::numbers::pi) + std::numbers::pi;
}


Track::Track()
{
    for (size_t i = 0; i < WINDOW_SIZE; i++)
    {
        hann_window[i] = 0.5 * (1.0 - cos(2.0 * 3.14159265358979323846 * i / (WINDOW_SIZE - 1)));
    }

    paused = true;

    wav_file = std::make_unique<WAVFile>("D:\\109193__juskiddink__leq-acappella.wav");

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
        std::array<Complex, WINDOW_SIZE> fft_buf;

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

                    const double in = static_cast<double>(input_buffer.Read());

                    processing_input_buffer[processing_input_buffer_pointer++] = in;
                    if (processing_input_buffer_pointer >= PROCESSING_BUFFER_SIZE)
                    {
                        // Wrap the circular buffer
                        // Notice: this is not the condition for starting a new FFT
                        processing_input_buffer_pointer = 0;
                    }

                    //get the output sample from the output buffer
                    // Scale the output down by the overlap factor (e.g. how many windows overlap per sample?)
                    double out = processing_output_buffer[processing_output_buffer_read_pointer] *
                        (static_cast<double>(HOP_SIZE) / WINDOW_SIZE);

                    // then clear the output sample in the buffer so it is ready for the next overlap-add
                    processing_output_buffer[processing_output_buffer_read_pointer] = 0;

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
                            processing_input_buffer, processing_input_buffer_pointer,
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
    std::array<double, PROCESSING_BUFFER_SIZE> const& in_buffer, size_t in_pointer,
    std::array<double, PROCESSING_BUFFER_SIZE>& out_buffer, size_t out_pointer,
    std::array<Complex, WINDOW_SIZE>& fft_buf)
{
    // Copy buffer into FFT input, starting one window ago
    for (int n = 0; n < WINDOW_SIZE; n++)
    {
        // Use modulo arithmetic to calculate the circular buffer index
        const size_t circular_buffer_index = (in_pointer + n - WINDOW_SIZE + PROCESSING_BUFFER_SIZE) % PROCESSING_BUFFER_SIZE;
        fft_buf[n] = Complex(in_buffer[circular_buffer_index]);
    }

    // Analysis window
    for (int n = 0; n < WINDOW_SIZE; n++)
    {
        fft_buf[n] = fft_buf[n] * hann_window[n];
    }

    // Process the FFT based on the time domain input
    fft(fft_buf);

    size_t max_bin_index = 0;
    double max_bin_value = 0;

    for (int n = 0; n < WINDOW_SIZE / 2; n++)
    {
        const Complex v = fft_buf[n];
        const double amplitude = std::sqrt(v.real() * v.real() + v.imag() * v.imag());
        const double phase = atan2(v.imag(), v.real());

        const double phase_diff_unwrapped = phase - last_input_phases[n];

        const double bin_centre_frequency = 2.0 * std::numbers::pi / (float)n / (float)WINDOW_SIZE;

        const double phase_diff = wrap_phase(phase_diff_unwrapped - bin_centre_frequency * HOP_SIZE);

        const double bin_deviation = phase_diff * (float)WINDOW_SIZE / (float)HOP_SIZE / (2.0 * std::numbers::pi);

        analysisFrequencies[n] = (double)n + bin_deviation;

        analysisMagnitudes[n] = amplitude;

        last_input_phases[n] = phase;

        if (amplitude > max_bin_value)
        {
            max_bin_value = amplitude;
            max_bin_index = n;
        }
    }

    const double max_freq = max_bin_index * 44100.0 / WINDOW_SIZE;
    std::cout << max_bin_index << " " << max_freq << "\n";

    // Robotise the output
    //for(int n = 0; n < WINDOW_SIZE; n++)
    //{
    //    const Complex v = fft_buf[n];
    //    const double amplitude = std::sqrt(v.real() * v.real() + v.imag() * v.imag());
    //    fft_buf[n] = Complex(amplitude, 0);
    //}

    // Run the inverse FFT
    ifft(fft_buf);

    // Synthesis window
    for (int n = 0; n < WINDOW_SIZE; n++)
    {
        fft_buf[n] = fft_buf[n] * hann_window[n];
    }

    // Add timeDomainOut into the output buffer starting at the write pointer
    for (int n = 0; n < WINDOW_SIZE; n++)
    {
        const size_t circular_buffer_index = (out_pointer + n) % PROCESSING_BUFFER_SIZE;
        out_buffer[circular_buffer_index] += fft_buf[n].real();
    }
}
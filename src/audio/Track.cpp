#include "Track.hpp"

#include <chrono>
#include <ratio>

#include "../Math.hpp"
#include "../signal/Windowing.hpp"

namespace Application
{
namespace Audio
{

const uint8_t DEVICE_NUM_CHANNELS = 1;

double wrap_phase(double phaseIn)
{
    if (phaseIn >= 0)
        return fmod(phaseIn + std::numbers::pi, 2.0 * std::numbers::pi) - std::numbers::pi;
    else
        return fmod(phaseIn - std::numbers::pi, -2.0 * std::numbers::pi) + std::numbers::pi;
}

void calculate_window(std::array<double, FFT_SIZE>& window, const size_t hop_size)
{
    // Recalculate window based on overlap factor
    const size_t OVERLAP = 4;

    size_t length = hop_size * OVERLAP;
    if (length > FFT_SIZE)
        length = FFT_SIZE;

    for (size_t i = 0; i < length; i++)
    {
        window[i] = 0.5 * (1.0 - cos(2.0 * std::numbers::pi * i / (length - 1)));
    }
}

Track::Track()
{
    processing_output_buffer_write_pointer = hop_size;
    paused = true;

    wav_file = std::make_shared<WAVFile>("D:\\ice.wav");
}

void Track::Generate(uint16_t* buffer, int length)
{
    const size_t channel = 0;

    for (int32_t i = 0; i < length; i++)
    {
        double position_whole;
        const double position_frac = modf(position, &position_whole);

        const uint32_t position_int = static_cast<uint32_t>(position_whole);

        const double samp0 = wav_file->ReadSample(position_int - 1, channel);
        const double samp1 = wav_file->ReadSample(position_int + 0, channel);
        const double samp2 = wav_file->ReadSample(position_int + 1, channel);
        const double samp3 = wav_file->ReadSample(position_int + 2, channel);

        const double in_sample = CatmullRomInterpolate(
            samp0, samp1, samp2, samp3,
            position_frac);

        position += speed_scale;

        processing_input_buffer[processing_input_buffer_pointer++] = in_sample;
        if (processing_input_buffer_pointer >= PROCESSING_BUFFER_SIZE)
        {
            // Wrap the circular buffer
            // Notice: this is not the condition for starting a new FFT
            processing_input_buffer_pointer = 0;
        }

        //get the output sample from the output buffer
        // Scale the output down by the overlap factor (e.g. how many windows overlap per sample?)
        double out = processing_output_buffer[processing_output_buffer_read_pointer] *
            (static_cast<double>(hop_size) / FFT_SIZE);

        // then clear the output sample in the buffer so it is ready for the next overlap-add
        processing_output_buffer[processing_output_buffer_read_pointer] = 0;

        // increment the read pointer in the output cicular buffer
        processing_output_buffer_read_pointer++;
        if (processing_output_buffer_read_pointer >= PROCESSING_BUFFER_SIZE)
        {
            processing_output_buffer_read_pointer = 0;
        }

        if (hop_size != hop_size_last)
        {
            calculate_window(window, hop_size);
        }

        // increment the hop counter and start a new FFT if we've reached the hop size
        if (++hop_counter >= hop_size)
        {
            hop_size_last = hop_size;
            hop_counter = 0;

            ProcessFFT(
                processing_input_buffer, processing_input_buffer_pointer,
                processing_output_buffer, processing_output_buffer_write_pointer);

            processing_output_buffer_write_pointer =
                (processing_output_buffer_write_pointer + hop_size) % PROCESSING_BUFFER_SIZE;
        }

        const uint16_t out_sample = static_cast<uint16_t>(out);

        const uint32_t out_idx = (i * DEVICE_NUM_CHANNELS) + channel;
        buffer[i] = out_sample;
    }
}

void Track::ProcessFFT(
    std::array<double, PROCESSING_BUFFER_SIZE> const& in_buffer, const size_t in_pointer,
    std::array<double, PROCESSING_BUFFER_SIZE>& out_buffer, const size_t out_pointer)
{
    // Copy buffer into FFT input, starting one window ago
    for (int n = 0; n < FFT_SIZE; n++)
    {
        // Use modulo arithmetic to calculate the circular buffer index
        const size_t circular_buffer_index = (in_pointer + n - FFT_SIZE + PROCESSING_BUFFER_SIZE) % PROCESSING_BUFFER_SIZE;
        fft_buf[n] = Complex(in_buffer[circular_buffer_index]);
    }

    // Analysis window
    for (int n = 0; n < FFT_SIZE; n++)
    {
        fft_buf[n] = fft_buf[n] * window[n];
    }

    // Process the FFT based on the time domain input
    fft(fft_buf);

    for (int n = 0; n <= FFT_SIZE / 2; n++)
    {
        const Complex v = fft_buf[n];
        const double amplitude = std::sqrt(v.real() * v.real() + v.imag() * v.imag());
        const double phase = atan2(v.imag(), v.real());

        const double phase_diff_unwrapped = phase - last_input_phases[n];
        const double bin_centre_frequency = 2.0 * std::numbers::pi * ((double)n / (double)FFT_SIZE);
        const double phase_diff = wrap_phase(phase_diff_unwrapped - bin_centre_frequency * hop_size);
        const double bin_deviation = phase_diff * (double)FFT_SIZE / (double)hop_size / (2.0 * std::numbers::pi);

        analysis_magnitudes[n] = amplitude;
        analysis_frequencies[n] = (double)n + bin_deviation;
        last_input_phases[n] = phase;
    }

    for (int n = 0; n <= FFT_SIZE / 2; n++)
    {
        synthesis_magnitudes[n] = 0;
        synthesis_frequencies[n] = 0;
    }

    const float pitch_shift = powf(2.0f, pitch_shift_semitones / 12.0f);

    // Handle pitch shift, storing frequencies in to new bins.
    for (int n = 0; n <= FFT_SIZE / 2; n++)
    {
        const uint32_t new_bin = static_cast<uint32_t>(floorf((pitch_shift * n) + 0.5f));

        // Ignore any bins above Nyquist
        if (new_bin <= FFT_SIZE / 2)
        {
            synthesis_magnitudes[new_bin] += analysis_magnitudes[n];
            synthesis_frequencies[new_bin] = analysis_frequencies[n] * pitch_shift;
        }
    }

    for (int n = 0; n <= FFT_SIZE / 2; n++)
    {
        const double amplitude = synthesis_magnitudes[n];

        // Fractional offset from bin center frequency
        const double bin_deviation = synthesis_frequencies[n] - n;

        // Convert to phase value
        double phase_diff = bin_deviation * 2.0 * std::numbers::pi * ((double)hop_size / (double)FFT_SIZE);

        const double bin_centre_frequency = 2.0 * std::numbers::pi * ((double)n / (double)FFT_SIZE);
        phase_diff += bin_centre_frequency * hop_size;

        const double out_phase = wrap_phase(last_output_phases[n] + phase_diff);

        const double real = amplitude * cos(out_phase);
        const double imag = amplitude * sin(out_phase);

        fft_buf[n] = Complex(real, imag);

        // Fill in mirror image of FFT
        if (n > 0 && n < FFT_SIZE / 2)
        {
            fft_buf[FFT_SIZE - n] = Complex(fft_buf[n].real(), -fft_buf[n].imag());
        }

        last_output_phases[n] = out_phase;
    }

#if false
    // Robotise the output
    for (int n = 0; n < FFT_SIZE; n++)
    {
        const Complex v = fft_buf[n];
        const double amplitude = std::sqrt(v.real() * v.real() + v.imag() * v.imag());
        fft_buf[n] = Complex(amplitude, 0);
    }

    // Whisperise the output
    for (int n = 0; n < FFT_SIZE; n++)
    {
        const Complex v = fft_buf[n];
        const double amplitude = std::sqrt(v.real() * v.real() + v.imag() * v.imag());
        const double phase = 2.0 * std::numbers::pi * (float)rand() / (float)RAND_MAX;
        const double real = amplitude * cos(phase);
        const double imag = amplitude * sin(phase);
        fft_buf[n] = Complex(real, imag);
    }
#endif

    // Run the inverse FFT
    ifft(fft_buf);

    // Synthesis window
    for (int n = 0; n < FFT_SIZE; n++)
    {
        fft_buf[n] = fft_buf[n] * window[n];
    }

    // Add timeDomainOut into the output buffer starting at the write pointer
    for (int n = 0; n < FFT_SIZE; n++)
    {
        const size_t circular_buffer_index = (out_pointer + n) % PROCESSING_BUFFER_SIZE;
        out_buffer[circular_buffer_index] += fft_buf[n].real();
    }
}


Track::~Track()
{

}

void Track::SetPaused(bool value)
{
    paused = value;
}

}
}

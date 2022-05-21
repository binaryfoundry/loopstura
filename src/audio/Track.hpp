#pragma once

#include <array>
#include <thread>
#include <memory>
#include <condition_variable>

#include "Waveform.hpp"
#include "Track.hpp"
#include "WAVFile.hpp"

#include "../signal/FFT.hpp"
#include "../concurrency/RingBuffer.hpp"
#include "../concurrency/SpinLock.hpp"

const uint32_t FFT_SIZE = 1024;
const uint32_t INPUT_BUFFER_SIZE = 16384;
const uint32_t SAMPLE_FREQ = 44100;
const size_t PROCESSING_BUFFER_SIZE = 16384;

constexpr bool IS_POW_2(int v)
{
    return v && ((v & (v - 1)) == 0);
}

static_assert(IS_POW_2(FFT_SIZE), "FFT_SIZE must be power of two.");

class Track
{

public:
    Track();
    virtual ~Track();

    virtual void InitComplete() = 0;

    float speed_scale = 1.0;
    int hop_size = 256;
    float pitch_shift_semitones = 0.0;

    void SetPaused(bool value);

    double Frequency()
    {
        return frequency;
    }

    std::shared_ptr<Waveform> Waveform()
    {
        return wav_file->waveform;
    }

    double PositionNormalized()
    {
        return wav_file->position / wav_file->ChannelSize();
    }

protected:
    int hop_size_last = -1;

    std::atomic<double> frequency = 0;

    std::atomic<bool> paused;
    std::shared_ptr<WAVFile> wav_file;

    std::atomic<bool> input_thread_running;
    std::unique_ptr<std::thread> input_thread;

    AudioSpinMutex input_mutex;
    std::condition_variable_any input_cond_var;

    RingBuffer<int16_t, INPUT_BUFFER_SIZE> input_buffer;

    std::atomic<bool> output_thread_running;
    std::unique_ptr<std::thread> output_thread;

    virtual void Queue(const void* data, uint32_t len) = 0;

    // Circular buffer and pointer for assembling a window of samples
    std::array<double, PROCESSING_BUFFER_SIZE> processing_input_buffer {};
    std::atomic<uint32_t> processing_input_buffer_pointer = 0;
    std::atomic<uint32_t> hop_counter = 0;

    // Circular buffer for collecting the output of the overlap-add process
    std::array<double, PROCESSING_BUFFER_SIZE> processing_output_buffer {};
    std::atomic<uint32_t> processing_output_buffer_write_pointer = 0;        // At minimum, write pointer stays one hop ahead of read pointer
    std::atomic<uint32_t> processing_output_buffer_read_pointer = 0;

    // FFT Processing
    std::array<Complex, FFT_SIZE> fft_buf;
    std::array<double, FFT_SIZE> window;

    std::array<double, FFT_SIZE / 2 + 1> analysis_magnitudes;
    std::array<double, FFT_SIZE / 2 + 1> analysis_frequencies;
    std::array<double, FFT_SIZE / 2 + 1> synthesis_magnitudes;
    std::array<double, FFT_SIZE / 2 + 1> synthesis_frequencies;
    std::array<double, FFT_SIZE> last_input_phases;
    std::array<double, FFT_SIZE> last_output_phases;

    void ProcessFFT(
        std::array<double, PROCESSING_BUFFER_SIZE> const& in_buffer, const size_t in_pointer,
        std::array<double, PROCESSING_BUFFER_SIZE>& out_buffer, const size_t out_pointer);
};

#pragma once

#include <array>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "Waveform.hpp"
#include "Track.hpp"
#include "WAVFile.hpp"

#include "../signal/FFT.hpp"
#include "../concurrency/RingBuffer.hpp"

const uint32_t WINDOW_SIZE = 1024;
const uint32_t INPUT_BUFFER_SIZE = 16384;
const uint32_t SAMPLE_FREQ = 44100;
const size_t PROCESSING_BUFFER_SIZE = 16384;

constexpr bool IS_POW_2(int v)
{
    return v && ((v & (v - 1)) == 0);
}

static_assert(IS_POW_2(WINDOW_SIZE), "WINDOW_SIZE must be power of two.");

class Track
{

public:
    Track();
    virtual ~Track();

    virtual void InitComplete() = 0;

    float speed_scale = 1.0;
    int hop_size = 256;

    std::shared_ptr<Waveform> waveform;
    void DrawWaveform(const double scale);

    void SetPaused(bool value);

    double Frequency()
    {
        return frequency;
    }

protected:
    std::atomic<double> frequency = 0;

    std::atomic<bool> paused;
    std::shared_ptr<WAVFile> wav_file;

    std::atomic<bool> input_thread_running;
    std::unique_ptr<std::thread> input_thread;

    std::mutex input_mutex;
    std::condition_variable input_cond_var;

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
};

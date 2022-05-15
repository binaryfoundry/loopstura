#pragma once

#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "Waveform.hpp"
#include "Track.hpp"
#include "WAVFile.hpp"

#include "../concurrency/RingBuffer.hpp"

const uint32_t INPUT_BUFFER_SIZE = 1024;
const uint32_t SAMPLE_FREQ = 44100;

class Track
{

public:
    Track();
    virtual ~Track();

    virtual void InitComplete() = 0;

    float speed_scale = 1.0;

    std::shared_ptr<Waveform> waveform;
    void DrawWaveform(const double scale);

protected:
    std::unique_ptr<WAVFile> wav_file;

    void ReadInput();

    std::atomic<bool> input_thread_running;
    std::unique_ptr<std::thread> input_thread;

    std::mutex input_mutex;
    std::condition_variable input_cond_var;

    RingBuffer<int16_t, INPUT_BUFFER_SIZE * 16> input_buffer;

    void WriteOutput();

    std::atomic<bool> output_thread_running;
    std::unique_ptr<std::thread> output_thread;

    virtual void Queue(const void* data, uint32_t len) = 0;

};

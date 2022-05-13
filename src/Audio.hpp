#pragma once

#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "Audio.hpp"
#include "concurrency/RingBuffer.hpp"
#include "file/WAVFile.hpp"

#define INPUT_BUFFER_SIZE 1024
#define SAMPLE_FREQ 44100

class Audio
{

public:
    Audio();
    virtual ~Audio();

    virtual void InitComplete() = 0;

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

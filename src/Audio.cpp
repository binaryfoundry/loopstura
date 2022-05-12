#include "Audio.hpp"

Audio::Audio()
{
    wav_file = std::make_unique<WAVFile>("D:\\file_example_WAV_10MG.wav");
}

Audio::~Audio()
{

}

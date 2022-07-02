#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "Track.hpp"

namespace Application
{
namespace Audio
{

class Device
{
public:
    void AddTrack(std::shared_ptr<Track> track);

    void Generate(uint16_t* buffer, int length);

    virtual void InitComplete() = 0;

private:
    std::vector<std::shared_ptr<Track>> tracks;
};

}
}

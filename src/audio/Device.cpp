#include "Device.hpp"

namespace Application
{
namespace Audio
{
    void Device::AddTrack(std::shared_ptr<Track> track)
    {
        tracks.push_back(track);
    }

    void Device::Generate(uint16_t* buffer, int length)
    {
        for (auto& track : tracks)
        {
            track->Generate(buffer, length);
        }
    }
}
}

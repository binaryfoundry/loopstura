#pragma once

#include "../Component.hpp"
#include "../Renderer.hpp"
#include "../audio/Track.hpp"

namespace Application
{
namespace Components
{
    class WaveformComponent : public Component
    {
    private:
        DisplayNodePtr waveform_0_bg;
        DisplayNodePtr waveform_0;
        DisplayNodePtr waveform_0_ft;
        TextureRGBA32FPtr waveform_texture;

        std::shared_ptr<Track> track;

    public:
        WaveformComponent(
            ContextPtr context,
            std::shared_ptr<Track> track,
            std::shared_ptr<Renderer> renderer,
            DisplayNodePtr parent);

        void Update(RenderState state) override;
    };
}
}

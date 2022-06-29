#include "WaveformComponent.hpp"

namespace Application
{
namespace Components
{
    WaveformComponent::WaveformComponent(
        ContextPtr context,
        std::shared_ptr<Track> track,
        std::shared_ptr<Renderer> renderer,
        DisplayNodePtr parent) :
        Component(parent),
        track(track)
    {
        *root->position = glm::vec2(0, 0);

        waveform_texture = renderer->MakeTextureFloat(
            4096, 1);

        waveform_0_ft = std::make_shared<DisplayNode>(
            context,
            root);

        *waveform_0_ft->alpha_margin = 1.0;
        *waveform_0_ft->nonlinearity = 12.0;
        *waveform_0_ft->gradient_0 = vec3(0.0);
        *waveform_0_ft->gradient_1 = vec3(0.0);
        *waveform_0_ft->roughness = 0.0;
        *waveform_0_ft->metalness = 1.0;
        waveform_0_ft->sdf_func = DisplayNodeSDFFunction::CYLINDER_HORIZONTAL;
        waveform_0_ft->blend_func = BlendFunc::ADDITIVE;

        waveform_0 = std::make_shared<DisplayNode>(
            context,
            root,
            waveform_texture);

        *waveform_0->scale = glm::vec2(500, 500);
        *waveform_0->texture_blend = 0.0;
        *waveform_0->gradient_0 = vec3(0.8, 0.49, 0.196);
        *waveform_0->gradient_1 = vec3(1.0, 0.843, 0.0);
        waveform_0->maintain_pixel_scaling_horizontal = true;
        waveform_0->sdf_func = DisplayNodeSDFFunction::WAVEFORM;

        waveform_0_bg = std::make_shared<DisplayNode>(
            context,
            root);

        *waveform_0_bg->alpha_margin = 1.0;
        *waveform_0_bg->nonlinearity = 8.0;
        *waveform_0_bg->gradient_0 = vec3(1.0);
        *waveform_0_bg->gradient_1 = vec3(0.25);
        waveform_0_bg->sdf_func = DisplayNodeSDFFunction::CYLINDER_HORIZONTAL;
    }

    void WaveformComponent::Update(RenderState state)
    {
        *waveform_0_bg->scale = glm::vec2(state.viewport.z, 200);
        *waveform_0->scale = glm::vec2(state.viewport.z, 200);
        *waveform_0_ft->scale = glm::vec2(state.viewport.z, 200);

        const double waveform_position = track->PositionNormalized();
        const std::shared_ptr<Waveform> waveform = track->Waveform();

        waveform->Fill(
            waveform_position,
            track->speed_scale,
            *waveform_texture->data);

        waveform_texture->Invalidate();
    }
}
}

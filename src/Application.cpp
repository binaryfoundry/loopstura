#include "Application.hpp"

#define IMGUI_HAS_DOCK

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "sdl/SDLTrack.hpp"

namespace Application
{
    Application::Application(
        ContextPtr context,
        std::shared_ptr<Renderer> renderer) :
        renderer(renderer),
        context(context)
    {
        track = std::make_unique<SDLTrack>();
        track->InitComplete();

        display_root = std::make_shared<DisplayNode>();

        renderer->RegisterRootNode(display_root);

        test_button = std::make_shared<DisplayNode>(
            context,
            display_root);

        *test_button->position = glm::vec2(300, 220);
        *test_button->scale = glm::vec2(100, 100);
        *test_button->brightness = 1.0;
        *test_button->alpha_margin = 20.0;
        test_button->sdf_func = 1;

        context->property_manager->AddTween(
            test_button->brightness,
            0.0f,
            1.0f,
            EasingFunction::EaseOutCubic);

        test_button_2 = std::make_shared<DisplayNode>(
            context,
            display_root);

        *test_button_2->position = glm::vec2(600, 220);
        *test_button_2->scale = glm::vec2(100, 100);
        *test_button_2->brightness = 0.0;
        *test_button_2->alpha_margin = 20.0;
        test_button_2->sdf_func = 2;

        waveform_texture = renderer->MakeTextureFloat(
            1024, 1);

        waveform_0 = std::make_shared<DisplayNode>(
            context,
            display_root,
            waveform_texture);

        *waveform_0->position = glm::vec2(0, 0);
        *waveform_0->scale = glm::vec2(500, 500);
        *waveform_0->texture_blend = 1.0;
        *waveform_0->gradient_0 = vec3(0.7);
        *waveform_0->gradient_1 = vec3(0.0);
        waveform_0->sdf_func = 3;
        waveform_0->mode = 1;


        waveform_0_bg = std::make_shared<DisplayNode>(
            context,
            display_root);

        *waveform_0_bg->position = glm::vec2(0, 0);
        *waveform_0_bg->alpha_margin = 1.0;
        *waveform_0_bg->gradient_0 = vec3(1.0);
        *waveform_0_bg->gradient_1 = vec3(0.5);
        waveform_0_bg->sdf_func = 4;

    }

    Application::~Application()
    {
    }

    void Application::DrawDock(bool* p_open)
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings;// | ImGuiWindowFlags_MenuBar;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        window_flags |= ImGuiWindowFlags_NoBackground;

        dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
        dockspace_flags |= ImGuiDockNodeFlags_NoTabBar;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", p_open, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, viewport->Size, dockspace_flags);

            if (!dock_created)
            {
                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags| ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.20f, nullptr, &dockspace_id);
                ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.20f, nullptr, &dockspace_id);
                ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.20f, nullptr, &dockspace_id);

                ImGui::DockBuilderDockWindow("Audio", dock_id_right);
                ImGui::DockBuilderFinish(dockspace_id);

                dock_created = true;
            }
        }

        ImGui::End();
    }

    void Application::Update()
    {
        ImGui::NewFrame();

        bool show = true;
        DrawDock(&show);

        ImGui::Begin("Audio");

        if (ImGui::Button("Play"))
        {
            track->SetPaused(false);
        }

        ImGui::SliderFloat("Speed", &track->speed_scale, -2, 2);
        ImGui::SliderInt("Hop Size", &track->hop_size, 64, 1024);
        ImGui::SliderFloat("Pitch Shift", &track->pitch_shift_semitones, -12, 12);

        ImGui::End();

        const float clamped_frame_duration = std::max(fps_timer.End(), 1.0f / 20.0f);

        context->Update(clamped_frame_duration);

        ImGuiIO& io = ImGui::GetIO();
        io.DeltaTime = clamped_frame_duration;

        fps_timer.Start();
    }

    void Application::Render()
    {
        RenderState state = renderer->state;

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(
            state.viewport.z,
            state.viewport.w);

        *waveform_0_bg->scale = glm::vec2(state.viewport.z, 200);
        *waveform_0->scale = glm::vec2(state.viewport.z, 200);

        const double waveform_position = track->PositionNormalized();
        const std::shared_ptr<Waveform> waveform = track->Waveform();
        waveform->Fill(waveform_position, *waveform_texture->data);
        waveform_texture->Invalidate();

        renderer->Draw(state);
    }
}

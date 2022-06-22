#include "Application.hpp"

#define IMGUI_HAS_DOCK

#include "components/WaveformComponent.hpp"
#include "components/ButtonComponent.hpp"

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
        track = std::make_shared<SDLTrack>();
        track->InitComplete();

        display_root = std::make_shared<DisplayNode>();

        renderer->RegisterRootNode(display_root);

        test_button = std::make_unique<Components::ButtonComponent>(
            context,
            glm::vec2(600, 320),
            glm::vec2(100, 100),
            DisplayNodeSDFFunction::CIRCLE,
            display_root);

        test_button_2 = std::make_unique<Components::ButtonComponent>(
            context,
            glm::vec2(300, 320),
            glm::vec2(100, 100),
            DisplayNodeSDFFunction::BOX_ROUNDED,
            display_root);

        waveform0 = std::make_unique<Components::WaveformComponent>(
            context,
            track,
            renderer,
            display_root);

        //context->property_manager->AddTween(
        //    test_button->brightness,
        //    0.0f,
        //    1.0f,
        //    EasingFunction::EaseOutCubic);
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

                ImGui::DockBuilderDockWindow("Audio", dock_id_bottom);
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

        waveform0->Update(state);

        renderer->Draw(state);
    }
}

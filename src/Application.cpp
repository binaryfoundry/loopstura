#include "Application.hpp"

#define IMGUI_HAS_DOCK

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui_plot/imgui_plot.h"

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

        TextureRGBA8Ptr background_texture = renderer->MakeTexture(
            "test.png");

        background_quad = std::make_shared<DisplayNode>(
            context,
            display_root,
            background_texture);

        renderer->RegisterNode(background_quad);

        context->property_manager->AddTween(
            background_quad->brightness,
            0.0f,
            1.0f,
            EasingFunction::EaseOutCubic);
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

                ImGui::DockBuilderDockWindow("Dear ImGui Demo", dock_id_left);
                ImGui::DockBuilderDockWindow("Audio", dock_id_right);
                ImGui::DockBuilderFinish(dockspace_id);

                dock_created = true;
            }
        }

        ImGui::End();
    }

    void Application::DrawPlot()
    {
        const size_t WAVEFORM_WINDOW_SIZE = 512;

        static std::vector<float> waveform_window_x(WAVEFORM_WINDOW_SIZE);
        static std::vector<float> waveform_window_y_max(WAVEFORM_WINDOW_SIZE);
        static std::vector<float> waveform_window_y_min(WAVEFORM_WINDOW_SIZE);

        static const float* y_data[] =
        {
            &waveform_window_y_min[0],
            &waveform_window_y_max[0]
        };

        static ImU32 colors[2] =
        {
            ImColor(255, 255, 0),
            ImColor(255, 255, 0)
        };

        const double waveform_position = track->PositionNormalized();
        const std::shared_ptr<Waveform> waveform = track->Waveform();
        waveform->FillX(waveform_position, waveform_window_x);
        waveform->FillYMax(waveform_position, waveform_window_y_max);
        waveform->FillYMin(waveform_position, waveform_window_y_min);

        static uint32_t selection_start = 0, selection_length = 0;

        ImGui::PlotConfig conf;
        conf.values.xs = &waveform_window_x[0];
        conf.values.count = WAVEFORM_WINDOW_SIZE;
        conf.values.ys_list = y_data;
        conf.values.ys_count = 2;
        conf.values.colors = colors;
        conf.scale.min = -1;
        conf.scale.max = 1;
        conf.tooltip.show = true;
        conf.grid_x.show = true;
        conf.grid_x.size = 128;
        conf.grid_x.subticks = 4;
        conf.grid_y.show = true;
        conf.grid_y.size = 0.5f;
        conf.grid_y.subticks = 5;
        conf.selection.show = true;
        conf.selection.start = &selection_start;
        conf.selection.length = &selection_length;
        conf.frame_size = ImVec2(WAVEFORM_WINDOW_SIZE, 200);
        ImGui::Plot("plot1", conf);

        conf.values.ys_list = nullptr;
        conf.selection.show = false;
        conf.values.ys = &waveform_window_y_max[0];
        conf.values.offset = selection_start;
        conf.values.count = selection_length;
        conf.line_thickness = 1.f;
    }

    void Application::Update()
    {
        ImGui::NewFrame();

        bool show = true;
        DrawDock(&show);

        ImGui::ShowDemoWindow();

        ImGui::Begin("Audio");

        if (ImGui::Button("Play"))
        {
            track->SetPaused(false);
        }

        ImGui::SliderFloat("Speed", &track->speed_scale, -2, 2);
        ImGui::SliderInt("Hop Size", &track->hop_size, 64, 1024);
        ImGui::SliderFloat("Pitch Shift", &track->pitch_shift_semitones, -12, 12);

        DrawPlot();

        ImGui::End();

        context->Update();
    }

    void Application::Render()
    {
        RenderState state = renderer->state;

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(
            state.viewport.z,
            state.viewport.w);

        *background_quad->scale = glm::vec2(
            renderer->state.viewport.z,
            renderer->state.viewport.w);

        renderer->Draw(state);
    }
}

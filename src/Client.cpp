#include "Client.hpp"

#define IMGUI_HAS_DOCK

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Application
{
    static std::initializer_list<float> quad_vertices_data
    {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f
    };

    static std::initializer_list<uint32_t> quad_indices_data
    {
         0, 1, 2, 2, 3, 0
    };


    Client::Client(
        ContextPtr context,
        std::shared_ptr<Renderer> renderer) :
        renderer(renderer),
        context(context)
    {
        quad_vertices = renderer->MakeVertexStream(
            StreamUsage::DYNAMIC,
            quad_vertices_data);

        quad_indices = renderer->MakeIndexStream(
            StreamUsage::DYNAMIC,
            quad_indices_data);

        quad_texture = renderer->MakeTexture(
            "test.png");

        quad_instance = renderer->MakeInstanceUI(
            context,
            quad_vertices,
            quad_indices,
            quad_texture);

        quad_instance->brightness->Set(
            1.0);

        quad_instance->gradient->Set(
            1.0);

        quad_instance->gradient_0->Set(
            vec3(0.36, 0.65, 0.74));

        quad_instance->gradient_1->Set(
            vec3(0.16, 0.27, 0.63));

        context->property_manager->AddTween(
            quad_instance->brightness,
            0.0f,
            1.0f,
            EasingFunction::EaseOutCubic);

        imgui_instance = renderer->MakeInstanceImgui(
            context);
    }

    Client::~Client()
    {
    }

    void Client::DrawDock(bool* p_open)
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

    void Client::Update()
    {
        ImGui::NewFrame();

        bool show = true;
        DrawDock(&show);

        ImGui::ShowDemoWindow();

        ImGui::Begin("Audio");
        if (ImGui::Button("Play") && track == nullptr)
        {
            track = std::make_unique<SDLTrack>();
            track->InitComplete();

        }
        ImGui::End();

        context->Update();
    }

    void Client::Render()
    {
        RenderState state = renderer->state;

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(
            state.viewport.z,
            state.viewport.w);

        renderer->Begin();

        vec3 scale = glm::vec3(
            renderer->state.viewport.z,
            renderer->state.viewport.w,
            1.0);

        state.view = glm::scale(
            state.view,
            scale);

        quad_instance->Draw(state);

        imgui_instance->Draw(state);

        renderer->End();
    }
}

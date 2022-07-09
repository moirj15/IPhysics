#include "UISystem.hpp"

#ifndef IMGUI_IMPL_OPENGL_LOADER_GLAD
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#endif
#include "../third_party/imgui/backends/imgui_impl_opengl3.h"
//#include "../third_party/imgui/backends/imgui_impl_dx11.h"
#include "../third_party/imgui/backends/imgui_impl_sdl.h"
#include "../third_party/imgui/imgui.h"
#include "Common.h"
#include "ResourceSystem.hpp"

#include <SDL2/SDL.h>

UISystem::UISystem(SDL_Window *window, ResourceSystem *resource_system) :
    m_mesh_file_name(256, 0), mSaveClicked(false), m_window(window), m_resource_system(resource_system)
{
    // TODO: put in base class
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    //  auto [device, context] = focus::GetDeviceAndContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplSDL2_InitForOpenGL(m_window, SDL_GL_GetCurrentContext());
    ImGui::StyleColorsClassic();
}

UISystem::~UISystem()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void UISystem::Update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window);
    ImGui::NewFrame();
    // Define the UI layout
    ImGui::Begin("Hello boi", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    {
        if (ImGui::Button("Load Mesh")) {
            m_resource_system->LoadMesh(m_mesh_file_name);
        }
        ImGui::SameLine();
        ImGui::InputText("Mesh File Name", m_mesh_file_name.data(), m_mesh_file_name.size());
        ImGui::InputFloat("Voxel Size", &mParameters.mVoxelSize);
        ImGui::Checkbox("Generate Hollow", &mParameters.mHollow);
        if (ImGui::Button("Save")) {
            mSaveClicked = true;
        }
        ImGui::SameLine();
    }

    ImGui::End();
    ImGui::EndFrame();

    // Render the UI
    // glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

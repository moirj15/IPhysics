//
// Created by Jimmy on 2/9/2020.
//

#include "VoxelizerUI.h"

#ifndef IMGUI_IMPL_OPENGL_LOADER_GLAD
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#endif
#include "../third_party/imgui/imgui.h"
#include "../third_party/imgui/backends/imgui_impl_win32.h"
#include "../third_party/imgui/backends/imgui_impl_opengl3.h"

#include <Common.h>

namespace VoxGen
{

VoxelizerUI::~VoxelizerUI()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
}

void VoxelizerUI::Init(const renderer::Window &window)
{
  // TODO: put in base class
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplWin32_Init(window.mWindowHandle);
//  ImGui_ImplWin32_InitForOpenGL(window->mGLWindow, true);
  ImGui_ImplOpenGL3_Init("#version 150");
  ImGui::StyleColorsClassic();
}

void VoxelizerUI::Update()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
  // Define the UI layout
  ImGui::Begin("Hello boi", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
  {
    if (ImGui::Button("Load Mesh"))
    {
      mLoadMeshClicked = true;
    }
    ImGui::SameLine();
    ImGui::InputText("Mesh File Name", mMeshFileName.data(), mMeshFileName.size());
    ImGui::InputFloat("Voxel Size", &mParameters.mVoxelSize);
    ImGui::Checkbox("Generate Hollow", &mParameters.mHollow);
    if (ImGui::Button("Generate Voxels"))
    {
      mGenerateVoxelsClicked = true;
    }
    if (ImGui::Button("Save"))
    {
      mSaveClicked = true;
    }
    ImGui::SameLine();
    ImGui::InputText("Voxel Mesh File Name", mVoxelMeshFileName.data(), mVoxelMeshFileName.size());
  }

  ImGui::End();
  ImGui::EndFrame();

  // Render the UI
  // glClear(GL_COLOR_BUFFER_BIT);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// TODO: might be a good idea to somehow register functions that occur when a ui element is clicked
std::optional<std::string> VoxelizerUI::LoadMeshClicked()
{
  if (mLoadMeshClicked)
  {
    mLoadMeshClicked = false;
    return {mMeshFileName};
  }
  return {};
}

std::optional<std::string> VoxelizerUI::SaveClicked()
{
  if (mSaveClicked)
  {
    mSaveClicked = false;
    return {mVoxelMeshFileName};
  }
  return {};
}
bool VoxelizerUI::GenerateVoxelsClicked()
{
  if (mGenerateVoxelsClicked)
  {
    mGenerateVoxelsClicked = false;
    return true;
  }
  return false;
}

} // namespace VoxGen

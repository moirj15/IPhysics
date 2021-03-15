#include "VoxelizerUI.h"

#ifndef IMGUI_IMPL_OPENGL_LOADER_GLAD
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#endif
#include "../third_party/imgui/backends/imgui_impl_opengl3.h"
//#include "../third_party/imgui/backends/imgui_impl_dx11.h"
#include "../third_party/imgui/backends/imgui_impl_sdl.h"
#include "../third_party/imgui/imgui.h"

#include <Common.h>
#include <Context.hpp>


VoxelizerUI::~VoxelizerUI()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void VoxelizerUI::Init(const focus::Window &window)
{
  // TODO: put in base class
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

//  auto [device, context] = focus::GetDeviceAndContext();
  ImGui_ImplSDL2_InitForD3D(window.mSDLWindow);
  ImGui_ImplOpenGL3_Init();
  ImGui::StyleColorsClassic();
}

void VoxelizerUI::Update(const focus::Window &window)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window.mSDLWindow);
  ImGui::NewFrame();
  // Define the UI layout
  ImGui::Begin("Hello boi", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
  {
    if (ImGui::Button("Load Mesh")) {
      mLoadMeshClicked = true;
    }
    ImGui::SameLine();
    ImGui::InputText("Mesh File Name", mMeshFileName.data(), mMeshFileName.size());
    ImGui::InputFloat("Voxel Size", &mParameters.mVoxelSize);
    ImGui::Checkbox("Generate Hollow", &mParameters.mHollow);
    if (ImGui::Button("Generate Voxels")) {
      mGenerateVoxelsClicked = true;
    }
    if (ImGui::Button("Save")) {
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
  if (mLoadMeshClicked) {
    mLoadMeshClicked = false;
    return {mMeshFileName};
  }
  return {};
}

std::optional<std::string> VoxelizerUI::SaveClicked()
{
  if (mSaveClicked) {
    mSaveClicked = false;
    return {mVoxelMeshFileName};
  }
  return {};
}
bool VoxelizerUI::GenerateVoxelsClicked()
{
  if (mGenerateVoxelsClicked) {
    mGenerateVoxelsClicked = false;
    return true;
  }
  return false;
}


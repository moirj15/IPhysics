//
// Created by Jimmy on 2/9/2020.
//

#include "VoxelizerUI.h"

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"
#include "../renderer/window.h"

#include <GL/glew.h>
namespace VoxGen
{

VoxelizerUI::~VoxelizerUI()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void VoxelizerUI::Init(Window *window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window->mGLWindow, true);
  ImGui_ImplOpenGL3_Init("#version 150");
}

void VoxelizerUI::Update()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  // Define the UI layout
  ImGui::Begin("Hello boi");

  ImGui::End();

  // Render the UI
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::optional<std::string> VoxelizerUI::LoadMeshClicked() const
{
  return std::optional<std::string>();
}

bool VoxelizerUI::SaveClicked() const
{
  return false;
}

VoxelizerUI::Parameters VoxelizerUI::GetParameters() const
{
  return VoxelizerUI::Parameters();
}

} // namespace VoxGen

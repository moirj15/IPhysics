#include "IPhysicsUI.h"
#ifndef IMGUI_IMPL_OPENGL_LOADER_GLAD
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#endif
#include "../third_party/imgui/backends/imgui_impl_opengl3.h"
//#include "../third_party/imgui/backends/imgui_impl_dx11.h"
#include "../third_party/imgui/backends/imgui_impl_sdl.h"
#include "../third_party/imgui/imgui.h"

#include <SDL.h>
#include <Common.h>
#include <glm/gtc/type_ptr.hpp>

namespace IPhysics
{
IPhysicsUI::~IPhysicsUI()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void IPhysicsUI::Init(const focus::Window &window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  auto &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui_ImplSDL2_InitForOpenGL(window.mSDLWindow, SDL_GL_GetCurrentContext());
  ImGui_ImplOpenGL3_Init();
  ImGui::StyleColorsClassic();
}

void IPhysicsUI::Update(const focus::Window &window)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window.mSDLWindow);
  ImGui::NewFrame();
  // Define the UI layout
  ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
  {
    ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
    mLoadObjectClicked = ImGui::Button("Load Object");
    ImGui::SameLine();
    ImGui::InputText("Object File Name", mObjectPath.data(), mObjectPath.size());
    ImGui::BeginGroup();
    {
      Physics::ObjectSettings objectSettings;
      if (mObjectSettings.find(mCurrentObjectSettingsHandle) != mObjectSettings.end())
      {
        objectSettings = mObjectSettings[mCurrentObjectSettingsHandle];
      }
      mSettingsFieldModified |=
          ImGui::InputFloat3("Position", glm::value_ptr(objectSettings.mPosition));

      //       mSettingsFieldModified |=
      //           ImGui::InputFloat3("Velocity", glm::value_ptr(objectSettings.mInitialVelocity));
      //
      //       mSettingsFieldModified |=
      //           ImGui::InputFloat3("Acceleration",
      //           glm::value_ptr(objectSettings.mInitialAccleration));

      //       mSettingsFieldModified |= ImGui::InputFloat("Mass", &objectSettings.mMass);
      //       mSettingsFieldModified |= ImGui::Checkbox("Treat as rigid",
      //       &objectSettings.mTreatAsRigid);
      if (mObjectSettings.find(mCurrentObjectSettingsHandle) != mObjectSettings.end())
      {
        mObjectSettings[mCurrentObjectSettingsHandle] = objectSettings;
      }
    }
    ImGui::EndGroup();
    ImGui::BeginGroup();
    {
      //            ImGui::Checkbox("Enable Gravity", &mPhysicsSettings.mEnableGravity);
      ImGui::Checkbox("Enable Extension", &mPhysicsSettings.mEnableExtension);
      //            ImGui::InputFloat("Mouse Force", &mPhysicsSettings.mMouseForce);
    }
    ImGui::EndGroup();
    ImGui::BeginGroup();
    {
      mStartSimulationClicked = ImGui::Button("Start Simulation");
      ImGui::SameLine();
      mStopSimulationClicked = ImGui::Button("Stop Simulation");
      ImGui::SameLine();
      mResetSimulationClicked = ImGui::Button("Reset Simulation");
    }
    ImGui::EndGroup();
  }

  ImGui::End();
  ImGui::EndFrame();

  // Render the UI
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::string *IPhysicsUI::LoadObjectClicked()
{
  if (mLoadObjectClicked)
  {
    mLoadObjectClicked = false;
    return &mObjectPath;
  }
  return nullptr;
}

bool IPhysicsUI::StartSimulationClicked()
{
  if (mStartSimulationClicked)
  {
    mStartSimulationClicked = false;
    return true;
  }
  return false;
}

bool IPhysicsUI::StopSimulationClicked()
{
  if (mStopSimulationClicked)
  {
    mStopSimulationClicked = false;
    return true;
  }
  return false;
}

bool IPhysicsUI::ResetSimulationClicked()
{
  if (mResetSimulationClicked)
  {
    mResetSimulationClicked = false;
    return true;
  }
  return false;
}

} // namespace IPhysics

#include "IPhysicsUI.h"

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"
#include "../renderer/window.h"

#include <glm/gtc/type_ptr.hpp>

namespace IPhysics
{
IPhysicsUI::~IPhysicsUI()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void IPhysicsUI::Init(Window *window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window->mGLWindow, true);
  ImGui_ImplOpenGL3_Init("#version 150");
  ImGui::StyleColorsClassic();
}

void IPhysicsUI::Update()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
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
      auto &objectSettings = Physics::ObjectSettings();
      if (mObjectSettings.find(mCurrentObjectSettingsHandle) != mObjectSettings.end())
      {
        objectSettings = mObjectSettings[mCurrentObjectSettingsHandle];
      }
      ImGui::InputFloat3("Position", glm::value_ptr(objectSettings.mPosition));
      ImGui::InputFloat3("Velocity", glm::value_ptr(objectSettings.mInitialVelocity));
      ImGui::InputFloat3("Acceleration", glm::value_ptr(objectSettings.mInitialAccleration));
      ImGui::InputFloat("Mass", &objectSettings.mMass);
      ImGui::Checkbox("Treat as rigid", &objectSettings.mTreatAsRigid);
    }
    ImGui::EndGroup();
    ImGui::BeginGroup();
    {
      ImGui::Checkbox("Enable Gravity", &mPhysicsSettings.mEnableGravity);
      ImGui::Checkbox("Enable Extension", &mPhysicsSettings.mEnableExtension);
      ImGui::InputFloat("Mouse Force", &mPhysicsSettings.mMouseForce);
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

std::optional<std::string> IPhysicsUI::LoadObjectClicked()
{
  if (mLoadObjectClicked)
  {
    mLoadObjectClicked = false;
    return mObjectPath;
  }
  return {};
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

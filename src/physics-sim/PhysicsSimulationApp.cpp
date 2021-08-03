#include "PhysicsSimulationApp.h"

#include "../third_party/imgui/backends/imgui_impl_sdl.h"
#include "../third_party/imgui/backends/imgui_impl_win32.h"

#include <SDL.h>
#include <Utils/Serialization.h>
#include <VoxelObjects/VoxelMesh.h>

namespace IPhysics
{
PhysicsSimulationApp::PhysicsSimulationApp() :
    mCamera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    mProjection(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f)),
    mWindow(focus::gContext->MakeWindow(1920, 1080)), mUI(), mDebugRenderer(new DebugRenderer()),
    mPhysicsEngine(mDebugRenderer), mRenderer(&mDeformationMeshManager)
{
  mUI.Init(mWindow);
  mPhysicsEngine.SetMeshManager(&mDeformationMeshManager);
}

PhysicsSimulationApp::~PhysicsSimulationApp() = default;

void PhysicsSimulationApp::Run()
{
  SDL_Event e;
  while (true) {
    while (SDL_PollEvent(&e) > 0) {
      // TODO: put into UI code
      ImGui_ImplSDL2_ProcessEvent(&e);
      if (e.type == SDL_QUIT) {
        return;
      }
    }
    SDL_PumpEvents();
    CollectInput(e);
    LoadObject();
    CollectUIInput();
    if (mCurrentState == State::Running) {
      mPhysicsEngine.Update(ImGui::GetIO().DeltaTime);
      ApplyDeformations();
    }
    Render();
  }
}

void PhysicsSimulationApp::LoadObject()
{
  auto optionalPath = mUI.LoadObjectClicked();
  if (optionalPath && fs::exists(*optionalPath)) {
    mPhysicsEngine.SetEngineSettings(mUI.GetPhysicsSettings());
    auto sceneMembers = ReadSceneFile(*optionalPath);
    for (const auto &sceneMember : sceneMembers) {
      auto [mesh, voxelMesh] = shared::DeSerialize(sceneMember.voxelMeshPath);
      auto handle = mDeformationMeshManager.AddMeshes(mesh, voxelMesh);
      mInitialMeshManager.AddMeshes(mesh, voxelMesh);
      mRenderer.LoadMesh(handle);
      mPhysicsEngine.SubmitObject(handle, sceneMember);
      mSceneMembers.emplace(handle, sceneMember);
    }
    //    mInitialMeshManager = mDeformationMeshManager;
  }
}
void PhysicsSimulationApp::CollectInput(const SDL_Event &e)
{
  auto &io = ImGui::GetIO();
  auto *keys = SDL_GetKeyboardState(nullptr);

  // Check if we want to apply a force with the mouse
  if (!io.WantCaptureMouse && io.MouseReleased[0] && SDL_GetModState() == KMOD_LSHIFT) {
    s32 x = 0, y = 0;
    SDL_GetMouseState(&x, &y);
    // Calculate the mouse position in normalized device coordinates
    const glm::vec2 mouseNDC(
        ((io.MousePos.x / (f32)mWindow.mWidth) - 0.5f) * 2.0f, -((io.MousePos.y / (f32)mWindow.mHeight) - 0.5f) * 2.0f);
    glm::vec3 rayStartNDC(mouseNDC, 0.0);
    glm::vec3 rayEndNDC(mouseNDC, 1.0);

    // Convert the ray start and end from NDC to world space
    auto invProjCamera = glm::inverse(mProjection * mCamera.CalculateMatrix());

    mPhysicsEngine.CastRayWithForce(rayStartNDC, rayEndNDC, invProjCamera, 1.0f);
  }
  if (io.WantCaptureKeyboard) {
    return;
  }
  if (!io.WantCaptureMouse && io.MouseDown[0] && SDL_GetModState() == KMOD_LSHIFT) {
    f32 screenWidth = f32(mWindow.mWidth);
    f32 screenHeight = f32(mWindow.mHeight);
    glm::vec2 mouseDelta((screenWidth / 2.0f) - io.MousePos.x, (screenHeight / 2.0f) - io.MousePos.y);
    mCamera.Rotate(mouseDelta * 10.0f * io.DeltaTime);
  }
  f32 boost = 1.0f;
  if (keys[SDL_SCANCODE_LSHIFT] && !io.WantCaptureKeyboard) {
    boost = 5.0f;
  }
  if (keys[SDL_SCANCODE_W] && !io.WantCaptureKeyboard) {
    mCamera.Move(glm::vec3(0.0f, 0.0f, 1.0f) * boost * io.DeltaTime);
  }
  if (keys[SDL_SCANCODE_S] && !io.WantCaptureKeyboard) {
    mCamera.Move(glm::vec3(0.0f, 0.0f, -1.0f) * boost * io.DeltaTime);
  }
  if (keys[SDL_SCANCODE_A] && !io.WantCaptureKeyboard) {
    mCamera.Move(glm::vec3(-1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (keys[SDL_SCANCODE_D] && !io.WantCaptureKeyboard) {
    mCamera.Move(glm::vec3(1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (keys[SDL_SCANCODE_E] && !io.WantCaptureKeyboard) {
    mCamera.Move(glm::vec3(0.0f, 1.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (keys[SDL_SCANCODE_Q] && !io.WantCaptureKeyboard) {
    mCamera.Move(glm::vec3(0.0f, -1.0f, 0.0f) * boost * io.DeltaTime);
  }
}

void PhysicsSimulationApp::CollectUIInput()
{
  if (mUI.StartSimulationClicked() && mCurrentState != State::Running) {
    if (mCurrentState == State::Stopped) {
      mPhysicsEngine.SetEngineSettings(mUI.GetPhysicsSettings());
      mPhysicsEngine.SetMeshManager(&mDeformationMeshManager);
      //      mDeformationMeshManager = mInitialMeshManager;
    }
    mCurrentState = State::Running;
  }
  if (mUI.StopSimulationClicked() && mCurrentState != State::Paused) {
    mCurrentState = State::Paused;
  }
  if (mUI.ResetSimulationClicked() && mCurrentState != State::Stopped) {
    mCurrentState = State::Stopped;
    mPhysicsEngine.Reset();
    mDeformationMeshManager = mInitialMeshManager;
    for (const auto &[handle, sceneMember] : mSceneMembers) {
      mPhysicsEngine.SubmitObject(handle, sceneMember);
    }
  }
}

void PhysicsSimulationApp::ApplyDeformations()
{
  for (auto handle : mDeformationMeshManager.GetAllHandles()) {
    auto *vMesh = mDeformationMeshManager.GetVoxelMesh(handle);
    auto *mesh = mDeformationMeshManager.GetMesh(handle);
    for (const auto &[key, voxel] : vMesh->voxels) {
      for (auto index : voxel.meshVertices) {
        mesh->SetVertex(index, mesh->GetVertex(index) + voxel.relativePositionDelta);
        // this is the good one
        // TODO: maybe doing this on the gpu isn't a good idea?
#if 0
        vMesh->mMesh->mOffsets.AccessCastBuffer(index) = voxel.mRelativePositionDelta;
#endif
        // TODO: Maybe move the copying into the physics engine so it isn't copied twice?

        //         vMesh->mMesh->mVertices.AccessCastBuffer(index) += voxel.mRelativePositionDelta;
        //         vMesh->mMesh->mOffsets.AccessCastBuffer(index) = voxel.mPositionRelativeToCenter;
      }
    }
    mRenderer.LoadMesh(handle);
  }
}

void PhysicsSimulationApp::Render()
{
  mRenderer.ClearScreen();

  // It might be better to just put the object positions in the mesh manager, but that can be experimented with later
  const auto &sceneMembers = (mCurrentState == State::Stopped) ? mSceneMembers : mPhysicsEngine.GetObjectSettings();
  for (const auto &[handle, member] : sceneMembers) {
    mRenderer.DrawMesh(handle, mCamera, glm::translate(glm::identity<glm::mat4>(), member.position));
    mRenderer.LoadDebugMesh(handle);
  }
  mDebugRenderer->Draw(mCamera.CalculateMatrix(), mProjection);

  if (mCurrentState == State::Running && mUI.GetPhysicsSettings().mEnableExtension) {

    for (auto handle : mDeformationMeshManager.GetAllHandles()) {
      auto *mesh = mDeformationMeshManager.GetMesh(handle);
      auto *vMesh = mDeformationMeshManager.GetVoxelMesh(handle);
      auto *originalMesh = mInitialMeshManager.GetMesh(handle);
      for (auto &[key, voxel] : vMesh->voxels) {
        std::unordered_map<u32, bool> alreadyCalculated;
        for (auto &bezierCurve : voxel.bezierCurves) {
          if (bezierCurve.controlPoints.size() == 3) {
            if (alreadyCalculated[bezierCurve.effectedPoints[0]]) {
              continue;
            }
            alreadyCalculated[bezierCurve.effectedPoints[0]] = true;
            u32 effectedPointIndex = bezierCurve.effectedPoints[0];
            //bezierCurve.CalculateQuadraticTValue();
            bezierCurve.Calc();
            auto p = voxel.CalculateFrom3Points(bezierCurve.controlPoints, bezierCurve.firstT);
            auto p2 = voxel.CalculateFrom3Points(bezierCurve.controlPoints, bezierCurve.secondT);
            auto p3 = voxel.CalculateFrom3Points(bezierCurve.controlPoints, 0.25f);
            mesh->SetVertex(effectedPointIndex, /*originalMesh->GetVertex(effectedPointIndex) +*/ p);

          } else {
            if (alreadyCalculated[bezierCurve.effectedPoints[0]]
                && alreadyCalculated[bezierCurve.effectedPoints[bezierCurve.effectedPoints.size() - 1]]) {
              continue;
            }
            u32 effectedPoint = alreadyCalculated[bezierCurve.effectedPoints[0]] ? bezierCurve.effectedPoints[1]
                                                                                 : bezierCurve.effectedPoints[0];
            alreadyCalculated[effectedPoint] = true;
            f32 t = alreadyCalculated[bezierCurve.effectedPoints[0]] ? bezierCurve.secondT : bezierCurve.firstT;

            mesh->SetVertex(effectedPoint,
                originalMesh->GetVertex(effectedPoint) + voxel.CalculateFrom4Points(bezierCurve.controlPoints, t));
          }
        }
      }
    }
  }
  mUI.Update(mWindow);
  mRenderer.UpdateScreen(mWindow);
}
} // namespace IPhysics

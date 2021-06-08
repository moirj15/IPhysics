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
    mWindow(focus::gContext->MakeWindow(1920, 1080)),
    /*mWindow(Renderer::InitAPI(1980, 1080, "Voxel Generator", false)),*/ mUI(),
    /*mRenderer(new Renderer::RendererFrontend(mWindow.get(), &mCamera)),*/
    /*mDB(new Renderer::DebugDrawer(mRenderer->GetBackend())),*/
    mDebugRenderer(new DebugRenderer()), mPhysicsEngine(mDebugRenderer), mRenderer(&mInitialMeshManager)
{
  mUI.Init(mWindow);
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
    if (mPhysicsSimulationRunning) {
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
    auto [mesh, voxelMesh] = shared::DeSerialize(*optionalPath);
    auto handle = mInitialMeshManager.AddMeshes(mesh, voxelMesh);
    mRenderer.LoadMesh(handle);

    // TODO: Modify the physics engine so it takes object setting modifications into account
    // mPhysicsEngine.SubmitObject(handle);
    //     mRenderer->RegisterMeshHandle(handle);
    mUI.SetCurrentObject(handle);
  }
}
void PhysicsSimulationApp::CollectInput(const SDL_Event &e)
{
  auto &io = ImGui::GetIO();
  auto *keys = SDL_GetKeyboardState(nullptr);

  // Check if we want to apply a force with the mouse
  if (!io.WantCaptureMouse && io.MouseReleased[0] && SDL_GetModState() & SDLK_LSHIFT) {
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
  if (!io.WantCaptureMouse && io.MouseDown[0] && (SDL_GetModState() & SDLK_LSHIFT) == 0) {
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
  auto handle = mUI.CurrentObject();
  if (handle == 0) {
    return;
  }
  // if (handle != 0 && mUI.SettingsFieldModified())
  //{
  //   VoxelMeshManager::Get().UpdateOriginalSettings(handle, mUI->GetCurrentObjectsSettings());
  // }
  // mPhysicsEngine.UpdateObject(handle, mUI.GetCurrentObjectsSettings().mPosition);
  if (mUI.StartSimulationClicked()) {
    mPhysicsSimulationRunning = true;
    mPhysicsEngine.Reset();
    mPhysicsEngine.SetEngineSettings(mUI.GetPhysicsSettings());
    mPhysicsEngine.SetObjectSettings(mUI.GetAllObjectSettings());
    // Only update the physics-engine's mesh manager if a new object has been added to the initial-meshmanager.
    // Otherwise, duplicate objects get added causing some interesting glitches to occur when the simulation restarts.
//    if (mDeformationMeshManager != mInitialMeshManager) {
      mDeformationMeshManager = mInitialMeshManager;
      mPhysicsEngine.SetMeshManager(&mDeformationMeshManager);
//    }
#if 0
    for (auto &[handle, setting] : mUI.GetAllObjectSettings())
    {
      auto *voxelMesh = VoxelMeshManager::Get().GetMesh(handle);
      //       for (auto &[key, voxel] : voxelMesh->mVoxels)
      //       {
      //         voxel.UpdateBezierCurves(setting.mPosition);
      //       }
#if 0
      VoxelMeshManager::Get().UpdateOriginalSettings(handle, setting);
#endif
      mPhysicsEngine.SubmitObject(handle);
    }
#endif
  }
  if (mUI.StopSimulationClicked()) {
    mPhysicsSimulationRunning = false;
  }
  if (mUI.ResetSimulationClicked()) {
    mPhysicsSimulationRunning = false;
    mPhysicsEngine.Reset();
#if 0
    VoxelMeshManager::Get().RestoreSettings();
#endif
    //     for (auto &[handle, _] : mUI->GetAllObjectSettings())
    //     {
    //       mPhysicsEngine->SubmitObject(handle);
    //     }
  }
}

void PhysicsSimulationApp::ApplyDeformations()
{
  for (auto handle : mInitialMeshManager.GetAllHandles()) {
    auto *vMesh = mInitialMeshManager.GetVoxelMesh(handle);
    auto *mesh = mInitialMeshManager.GetMesh(handle);
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
  if (mPhysicsSimulationRunning) {
    // TODO: refactor
    for (const auto &[handle, settings] : mPhysicsEngine.GetObjectSettings()) {
      mRenderer.DrawMesh(handle, mCamera, glm::translate(glm::identity<glm::mat4>(), settings.mPosition));
      mRenderer.LoadDebugMesh(handle);
      //      mRenderer.DrawDebugVoxels(handle, mCamera, glm::translate(glm::identity<glm::mat4>(),
      //      settings.mPosition));
    }
    mDebugRenderer->Draw(mCamera.CalculateMatrix(), mProjection);
  } else {
    for (const auto &[handle, settings] : mUI.GetAllObjectSettings()) {
      mRenderer.DrawMesh(handle, mCamera, glm::translate(glm::identity<glm::mat4>(), settings.mPosition));
      mRenderer.LoadDebugMesh(handle);
      //      mRenderer.DrawDebugVoxels(handle, mCamera, glm::translate(glm::identity<glm::mat4>(),
      //      settings.mPosition));
    }
  }

  if (mPhysicsSimulationRunning && mUI.GetPhysicsSettings().mEnableExtension) {

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
            mesh->SetVertex(
                effectedPointIndex, originalMesh->GetVertex(effectedPointIndex)
                                        + voxel.CalculateFrom3Points(bezierCurve.controlPoints, bezierCurve.firstT));

#if 0
            mesh->mOffsets.AccessCastBuffer(bezierCurve.mEffectedPoints[0]) =
                voxel.CalculateFrom3Points(bezierCurve.mControlPoints, bezierCurve.mFirstT);
#endif

            //////////////////////////////////////////////////////////////////////////
            //             auto p = bezierCurve.mControlPoints;
            //             auto a = p[0] - 2.0f * p[1] + p[2];
            //             auto b = -2.0f * p[0] + 2.0f * p[1];
            //             auto c = -p[1] + p[0];
            //             auto posT = (-b + glm::sqrt(glm::pow(b, glm::vec3(2.0f)) - 4.0f * a * c))
            //             / (2.0f * a); auto negT = (-b - glm::sqrt(glm::pow(b, glm::vec3(2.0f))
            //             - 4.0f * a * c)) / (2.0f * a); f32 best = posT.x; for (u32 i = 0; i < 3;
            //             i++)
            //             {
            //               if (posT[i] <= 1.0f && posT[i] >= 0.0f)
            //               {
            //                 best = posT[i];
            //               }
            //               if (negT[i] <= 1.0f && negT[i] >= 0.0f)
            //               {
            //                 best = posT[i];
            //               }
            //             }
            //////////////////////////////////////////////////////////////////////////
            //             auto tmp = voxel.CalculateFrom3Points(bezierCurve.mControlPoints, best);
          } else // if (bezierCurve.mControlPoints.size() == 4)
          {
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
#if 0
            mesh->mOffsets.AccessCastBuffer(effectedPoint) =
                voxel.CalculateFrom4Points(bezierCurve.mControlPoints, t);
#endif
            //////////////////////////////////////////////////////////////////////////
            //             auto points = bezierCurve.mControlPoints;
            //             f32 bestT = 0.0f;
            //             for (u32 i = 0; i < 3; i++)
            //             {
            //
            //               std::complex<f32> a =
            //                   -points[0][i] + (3.0f * points[1][i]) - (3.0f * points[2][i]) +
            //                   points[3][i];
            //               std::complex<f32> b = 3.0f * points[0][i] - 6.0f * points[1][i] + 3.0f
            //               * points[2][i]; std::complex<f32> c = -3.0f * points[0][i] + 3.0f *
            //               points[1][i]; std::complex<f32> d = points[0][i] - points[index][i];
            //
            //               auto p = -b / (3.0f * a);
            //               auto q = pow(p, 3.0f) + (b * c - 3.0f * a * d) / (6.0f * pow(a, 2.0f));
            //               auto r = c / (3.0f * a);
            //
            //               auto t = pow(q + pow(q * q + pow(r - (p * p), 3.0f), 1.0f / 2.0f), 1.0f
            //               / 3.0f)
            //                        + pow(q - pow((q * q) + pow(r - (p * p), 3.0f), 1.0f
            //                        / 2.0f), 1.0f / 3.0f)
            //                        + p;
            //               if (!isnan(t.real()))
            //               {
            //                 bestT = t.real();
            //               }
            //               //               for (u32 i = 0; i < 3; i++)
            //               //               {
            //               auto tn = voxel.CalculateFrom4Points(points, t.real());
            //               //               printf("tn[%d] = %s\n", i,
            //               glm::to_string(tn).c_str());
            //               //               }
            //             }
            //             auto tmp = voxel.CalculateFrom4Points(bezierCurve.mControlPoints, bestT);
            //////////////////////////////////////////////////////////////////////////
            //             printf("actual = %s\n", glm::to_string(points[index]).c_str());
            //             printf("\n");
          }
        }
      }
    }
  }
  mUI.Update(mWindow);
  mRenderer.UpdateScreen(mWindow);

  // TODO: add debug check
  //   QuickCastBuffer<f32, glm::vec3> points;
  //   for (auto &[handle, vMesh, settings] : VoxelMeshManager::Get().GetAllMeshes())
  //   {
  //     for (const auto &[key, voxel] : vMesh->mVoxels)
  //     {
  //       for (const auto &bezierCurve : voxel.mBezierCurves)
  //       {
  //         for (const auto &cp : bezierCurve.mControlPoints)
  //         {
  //           points.CastBufferPushBack(cp);
  //         }
  //       }
  //     }
  //   }
  //   mRenderer->DrawPoints(points);

#if 0
  mRenderer->Draw();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  mDB->Draw(mCamera.CalculateMatrix(), mProjection);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  mUI->Update();
  glfwSwapBuffers(mWindow->mGLWindow);
#endif
}
} // namespace IPhysics

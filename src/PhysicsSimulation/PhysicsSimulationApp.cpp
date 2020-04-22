#include "PhysicsSimulationApp.h"

#include "../../imgui/imgui.h"
#include "../PhysicsEngine/physics.h"
#include "../renderer/DebugDrawer.h"
#include "../renderer/RendererBackend.h"
#include "../renderer/RendererFrontend.h"
#include "../renderer/window.h"
#include "../utils/Serialization.h"
#include "../voxelObjects/VoxelMesh.h"
#include "IPhysicsUI.h"

#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

namespace IPhysics
{
PhysicsSimulationApp::PhysicsSimulationApp() :
    mCamera(
        glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    mProjection(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f)),
    mWindow(Renderer::InitAPI(1980, 1080, "Voxel Generator", false)), mUI(new IPhysicsUI()),
    mRenderer(new Renderer::RendererFrontend(mWindow.get(), &mCamera)),
    mDB(new Renderer::DebugDrawer(mRenderer->GetBackend())),
    mPhysicsEngine(new Physics::PhysicsEngine(mDB))
{
  mUI->Init(mWindow.get());
  mRenderer->SetProjection(mProjection);
}

PhysicsSimulationApp::~PhysicsSimulationApp() = default;

void PhysicsSimulationApp::Run()
{

  while (!mWindow->ShouldClose())
  {
    auto &io = ImGui::GetIO();
    LoadObject();
    CollectInput();
    CollectUIInput();
    if (mPhysicsSimulationRunning)
    {
      mPhysicsEngine->Update(io.DeltaTime);
      ApplyDeformations();
    }
    Render();
  }
}

void PhysicsSimulationApp::LoadObject()
{
  auto optionalPath = mUI->LoadObjectClicked();
  if (optionalPath && fs::exists(*optionalPath))
  {
    auto *voxelMesh = Utils::DeSerialize(*optionalPath);
    voxelMesh->mMesh->mOffsets.SetBufferSize(voxelMesh->mMesh->mVertices.BufferSize());
    u32 handle = VoxelMeshManager::Get().SubmitMesh(voxelMesh);
    // TODO: Modify the physics engine so it takes object setting modifications into account
    //     mPhysicsEngine->SubmitObject(handle);
    mRenderer->RegisterMeshHandle(handle);
    mUI->SetCurrentObject(handle);
  }
}

void PhysicsSimulationApp::CollectInput()
{
  glfwPollEvents();
  auto &io = ImGui::GetIO();

  // Check if we want to apply a force with the mouse
  if (!io.WantCaptureMouse && io.MouseReleased[0] && io.KeysDown[GLFW_KEY_LEFT_CONTROL])
  {
    f32 screenWidth = f32(mWindow->GetWidth());
    f32 screenHeight = f32(mWindow->GetHeight());
    // Calculate the mouse position in normalized device coordinates
    const glm::vec2 mouseNDC(
        ((io.MousePos.x / screenWidth) - 0.5f) * 2.0f,
        -((io.MousePos.y / screenHeight) - 0.5f) * 2.0f);
    glm::vec3 rayStartNDC(mouseNDC, 0.0);
    glm::vec3 rayEndNDC(mouseNDC, 1.0);

    // Convert the ray start and end from NDC to world space
    auto invProjCamera = glm::inverse(mProjection * mCamera.CalculateMatrix());

    mPhysicsEngine->CastRayWithForce(rayStartNDC, rayEndNDC, invProjCamera, 1.0f);
  }
  f32 boost = 1.0f;
  if (io.KeysDown[GLFW_KEY_LEFT_SHIFT] && !io.WantCaptureKeyboard)
  {
    boost = 5.0f;
  }
  if (io.KeysDown[GLFW_KEY_W] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(0.0f, 0.0f, 1.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_S] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(0.0f, 0.0f, -1.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_A] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(-1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_D] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_E] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(0.0f, 1.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_Q] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(0.0f, -1.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (!io.WantCaptureMouse && io.MouseDown[0] && !io.KeysDown[GLFW_KEY_LEFT_CONTROL])
  {
    f32 screenWidth = f32(mWindow->GetWidth());
    f32 screenHeight = f32(mWindow->GetHeight());
    glm::vec2 mouseDelta(
        (screenWidth / 2.0f) - io.MousePos.x, (screenHeight / 2.0f) - io.MousePos.y);
    mCamera.Rotate(mouseDelta * 10.0f * io.DeltaTime);
  }
}

void PhysicsSimulationApp::CollectUIInput()
{
  auto handle = mUI->CurrentObject();
  if (handle != 0 && mUI->SettingsFieldModified())
  {
    VoxelMeshManager::Get().UpdateOriginalSettings(handle, mUI->GetCurrentObjectsSettings());
  }
  if (mUI->StartSimulationClicked())
  {
    mPhysicsSimulationRunning = true;
    mPhysicsEngine->SetEngineSettings(mUI->GetPhysicsSettings());
    for (auto &[handle, setting] : mUI->GetAllObjectSettings())
    {
      auto *voxelMesh = VoxelMeshManager::Get().GetMesh(handle);
      //       for (auto &[key, voxel] : voxelMesh->mVoxels)
      //       {
      //         voxel.UpdateBezierCurves(setting.mPosition);
      //       }
      VoxelMeshManager::Get().UpdateOriginalSettings(handle, setting);
      mPhysicsEngine->SubmitObject(handle);
    }
  }
  if (mUI->StopSimulationClicked())
  {
    mPhysicsSimulationRunning = false;
  }
  if (mUI->ResetSimulationClicked())
  {
    mPhysicsSimulationRunning = false;
    mPhysicsEngine->Reset();
    VoxelMeshManager::Get().RestoreSettings();
    for (auto &[handle, _] : mUI->GetAllObjectSettings())
    {
      mPhysicsEngine->SubmitObject(handle);
    }
  }
}

void PhysicsSimulationApp::ApplyDeformations()
{
  for (auto &[handle, vMesh, settings] : VoxelMeshManager::Get().GetAllMeshes())
  {
    for (const auto &[key, voxel] : vMesh->mVoxels)
    {
      for (auto index : voxel.mMeshVertices)
      {
        // this is the good one
        // TODO: maybe doing this on the gpu isn't a good idea?
        vMesh->mMesh->mOffsets.AccessCastBuffer(index) = voxel.mRelativePositionDelta;
        // TODO: Maybe move the copying into the physics engine so it isn't copied twice?

        //         vMesh->mMesh->mVertices.AccessCastBuffer(index) += voxel.mRelativePositionDelta;
        //         vMesh->mMesh->mOffsets.AccessCastBuffer(index) = voxel.mPositionRelativeToCenter;
      }
    }
    mRenderer->UpdateMesh(handle);
  }
}

void PhysicsSimulationApp::Render()
{
  mRenderer->Clear();
  if (mPhysicsSimulationRunning)
  {

    for (auto &[handle, vMesh, settings] : VoxelMeshManager::Get().GetAllMeshes())
    {
      auto *mesh = vMesh->mMesh;
      for (auto &[key, voxel] : vMesh->mVoxels)
      {
        std::unordered_map<u32, bool> alreadyCalculated;
        for (auto &bezierCurve : voxel.mBezierCurves)
        {
          if (bezierCurve.mControlPoints.size() == 3)
          {
            if (alreadyCalculated[bezierCurve.mEffectedPoints[0]])
            {
              continue;
            }
            alreadyCalculated[bezierCurve.mEffectedPoints[0]] = true;
            // just assuming t = .5 for now, need to actually calculate this later
            //             mesh->mVertices.AccessCastBuffer(bezierCurve.mEffectedPoints[0]) =
            //                 voxel.CalculateFrom3Points(bezierCurve.mControlPoints, 0.5f);
            auto p = bezierCurve.mControlPoints;
            auto a = p[0] - 2.0f * p[1] + p[2];
            auto b = -2.0f * p[0] + 2.0f * p[1];
            auto c = -p[1] + p[0];
            auto posT = (-b + glm::sqrt(glm::pow(b, glm::vec3(2.0f)) - 4.0f * a * c)) / (2.0f * a);
            auto negT = (-b - glm::sqrt(glm::pow(b, glm::vec3(2.0f)) - 4.0f * a * c)) / (2.0f * a);
            for (u32 i = 0; i < 3; i++)
            {
              auto tn = voxel.CalculateFrom3Points(p, negT[i]);
              auto tp = voxel.CalculateFrom3Points(p, posT[i]);
              printf("tn[%d] = %s\n", i, glm::to_string(tn).c_str());
              printf("tp[%d] = %s\n", i, glm::to_string(tp).c_str());
            }
            printf("actual = %s\n", glm::to_string(p[1]).c_str());

            //////////////////////////////////////////////////////////////////////////
            //             f32 totalLength =
            //                 glm::length(bezierCurve.mControlPoints[0] -
            //                 bezierCurve.mControlPoints[2]);
            //             if (totalLength == 0.0f)
            //             {
            //               totalLength = 1.0f;
            //             }
            //             // TODO: need to have the control points ordered from left to right
            //
            //             f32 t;
            //             if (glm::any(
            //                     glm::lessThan(bezierCurve.mControlPoints[0],
            //                     bezierCurve.mControlPoints[1])))
            //             {
            //               t = glm::length(bezierCurve.mControlPoints[1] -
            //               bezierCurve.mControlPoints[0])
            //                   / totalLength;
            //             }
            //             else
            //             {
            //               t = glm::length(bezierCurve.mControlPoints[1] -
            //               bezierCurve.mControlPoints[2])
            //                   / totalLength;
            //             }
            //
            //             auto result = voxel.CalculateFrom3Points(bezierCurve.mControlPoints, t
            //             / 2.0f); mesh->mVertices.AccessCastBuffer(bezierCurve.mEffectedPoints[0])
            //             = result;
            //////////////////////////////////////////////////////////////////////////

            //             mesh->mVertices.AccessCastBuffer(bezierCurve.mEffectedPoints[0]) =
            //                 voxel.CalculateFrom3Points(bezierCurve.mControlPoints,
            //                 bezierCurve.mTStart);
            // mesh->mOffsets.AccessCastBuffer(bezierCurve.mEffectedPoints[0]) =
            //    voxel.CalculateFrom3Points(bezierCurve.mControlPoints, 0.5f);
          }
          else // if (bezierCurve.mControlPoints.size() == 4)
          {
            if (alreadyCalculated[bezierCurve.mEffectedPoints[0]]
                && alreadyCalculated[bezierCurve
                                         .mEffectedPoints[bezierCurve.mEffectedPoints.size() - 1]])
            {
              continue;
            }
            u32 effectedPoint = alreadyCalculated[bezierCurve.mEffectedPoints[0]]
                                    ? bezierCurve.mEffectedPoints[1]
                                    : bezierCurve.mEffectedPoints[0];
            alreadyCalculated[effectedPoint] = true;
            u32 tIndex = alreadyCalculated[bezierCurve.mEffectedPoints[0]] ? 2 : 1;
            f32 totalLength =
                glm::length(bezierCurve.mControlPoints[0] - bezierCurve.mControlPoints[3]);
            if (totalLength == 0.0f)
            {
              totalLength = 1.0f;
            }
            f32 t = glm::length(bezierCurve.mControlPoints[tIndex] - bezierCurve.mControlPoints[0])
                    / totalLength;
            //             f32 t = alreadyCalculated[bezierCurve.mEffectedPoints[0]] ?
            //             bezierCurve.mTStart
            //                                                                       :
            //                                                                       bezierCurve.mTEnd;
            auto result = voxel.CalculateFrom4Points(bezierCurve.mControlPoints, t);
            mesh->mVertices.AccessCastBuffer(effectedPoint) = result;

            // mesh->mOffsets.AccessCastBuffer(effectedPoint) =
            //    voxel.CalculateFrom4Points(bezierCurve.mControlPoints, t);
          }
        }
      }
    }
  }

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

  mRenderer->Draw();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  mDB->Draw(mCamera.CalculateMatrix(), mProjection);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  mUI->Update();
  glfwSwapBuffers(mWindow->mGLWindow);
}
} // namespace IPhysics

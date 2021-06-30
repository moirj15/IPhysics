#pragma once

#include "DebugRenderer.h"
#include "IPhysicsUI.h"
#include "Physics.h"
#include "Scene.h"

#include <Common.h>
#include <Context.hpp>
#include <Renderer/Camera.h>
#include <Renderer/Renderer.h>
#include <SDL.h>
#include <VoxelObjects/MeshManager.h>
#include <glm/mat4x4.hpp>
#include <memory>

struct Window;

namespace IPhysics
{

class IPhysicsUI;

class PhysicsSimulationApp
{
  Camera mCamera;
  glm::mat4 mProjection;
  focus::Window mWindow;
  IPhysicsUI mUI;
  DebugRenderer *mDebugRenderer;
  Physics::PhysicsEngine mPhysicsEngine;

  MeshManager mInitialMeshManager;
  MeshManager mDeformationMeshManager;
  std::unordered_map<MeshHandle, SceneMember> mSceneMembers;
  shared::Renderer mRenderer;

  enum class State {
    Running,
    Stopped,
    Paused,
  } mCurrentState = State::Stopped;

public:
  PhysicsSimulationApp();
  ~PhysicsSimulationApp();

  void Run();

private:
  void LoadObject();
  void CollectInput(const SDL_Event &e);
  void CollectUIInput();
  void ApplyDeformations();
  void Render();
};

} // namespace IPhysics

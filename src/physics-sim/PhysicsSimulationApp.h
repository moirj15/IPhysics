#pragma once

#include <VoxelObjects/MeshManager.h>

#include <Common.h>
#include <Renderer/Camera.h>
#include <glm/mat4x4.hpp>
#include <memory>

#include "Physics.h"
#include "IPhysicsUI.h"
#include <Context.hpp>
#include <Renderer/Renderer.h>
#include <SDL.h>

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
  Renderer::DebugDrawer *mDB;
  Physics::PhysicsEngine mPhysicsEngine;
  bool mPhysicsSimulationRunning = false;

  MeshManager mMeshManager;
  shared::Renderer mRenderer;

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

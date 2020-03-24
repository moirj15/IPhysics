#pragma once

#include "../common.h"
#include "../renderer/camera.h"
#include "../utils/VoxelMeshManager.h"

#include <glm/mat4x4.hpp>
#include <memory>

struct Window;

namespace Renderer
{
class RendererFrontend;
class DebugDrawer;
} // namespace Renderer

namespace Physics
{
class ObjectSettings;
class EngineSettings;
class PhysicsEngine;
} // namespace Physics

namespace IPhysics
{

class IPhysicsUI;

class PhysicsSimulationSystem
{
  Camera mCamera;
  glm::mat4 mProjection;
  VMeshHandle mHandle = 0;
  std::unique_ptr<Window> mWindow;
  std::unique_ptr<IPhysicsUI> mUI;
  std::unique_ptr<Renderer::RendererFrontend> mRenderer;
  Renderer::DebugDrawer *mDB;
  std::unique_ptr<Physics::PhysicsEngine> mPhysicsEngine;
  bool mPhysicsSimulationRunning{false};

public:
  PhysicsSimulationSystem();
  ~PhysicsSimulationSystem();

  void Run();

private:
  void LoadObject();
  void CollectInput();
  void CollectUIInput();
  void ApplyDeformations();
  void Render();
};

} // namespace IPhysics

#pragma once

#include "../common.h"
#include "../renderer/camera.h"

#include <memory>

struct Window;

namespace Renderer
{
class RendererFrontend;
}

namespace Physics
{
class ObjectSettings;
class EngineSettings;
class PhysicsEngine;
} // namespace Physics

namespace IPhysics
{

class IPhysicsUI;

class System
{
  Camera mCamera;
  std::unique_ptr<Window> mWindow;
  std::unique_ptr<IPhysicsUI> mUI;
  std::unique_ptr<Renderer::RendererFrontend> mRenderer;
  std::unique_ptr<Physics::PhysicsEngine> mPhysicsEngine;

public:
  System();
  ~System();

  void Run();

private:
  void Render();
};

} // namespace IPhysics

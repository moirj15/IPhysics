#pragma once

#include <LinearMath/btIDebugDraw.h>

namespace Renderer
{

class RenderereBackend;

class DebugDrawer : public btIDebugDraw
{
  RenderereBackend *mBackend;

public:
  DebugDrawer(RenderereBackend *backend) : mBackend(backend)
  {
  }
  ~DebugDrawer() = default;

  void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;
};

} // namespace Renderer

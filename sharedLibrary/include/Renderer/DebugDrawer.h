#pragma once

#include <Common.h>
#include "Mesh.h"

#include <LinearMath/btIDebugDraw.h>

namespace Renderer
{

class RendererBackend;

class DebugDrawer : public btIDebugDraw
{
  RendererBackend *mBackend;
  Mesh mMesh;

public:
  DebugDrawer(RendererBackend *backend) : mBackend(backend)
  {
  }
  ~DebugDrawer() = default;

  void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;

  void Draw(const glm::mat4 &camera, const glm::mat4 &projection);
  void drawContactPoint(
      const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime,
      const btVector3 &color) override;
  void reportErrorWarning(const char *warningString) override;
  void draw3dText(const btVector3 &location, const char *textString) override;
  void setDebugMode(int debugMode) override;
  int getDebugMode() const override;
};

} // namespace Renderer

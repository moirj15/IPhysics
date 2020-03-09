#include "DebugDrawer.h"

#include "RendererBackend.h"

namespace Renderer
{
void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
  mMesh.mVertecies.push_back(from.x());
  mMesh.mVertecies.push_back(from.y());
  mMesh.mVertecies.push_back(from.z());

  mMesh.mVertecies.push_back(to.x());
  mMesh.mVertecies.push_back(to.y());
  mMesh.mVertecies.push_back(to.z());

  mMesh.mIndecies.push_back(mMesh.mIndecies.size());
  mMesh.mIndecies.push_back(mMesh.mIndecies.size());
}

void DebugDrawer::Draw(const glm::mat4 &camera, const glm::mat4 &projection)
{
  // mBackend->ClearCommandQueue();
  auto handle = mBackend->SubmitMesh(&mMesh);
  std::vector<ShaderData> shaderData({ShaderData("color", {1.0, 1.0, 1.0}),
                                      ShaderData("camera", camera),
                                      ShaderData("projection", projection)});
  DrawCommand dc(CommandType::DrawLine, handle, shaderData);
  mBackend->SubmitCommand(dc);
  mBackend->Draw();
  mBackend->RemoveMesh(handle);
  mMesh.Clear();
}

void DebugDrawer::drawContactPoint(
    const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime,
    const btVector3 &color)
{
}

void DebugDrawer::reportErrorWarning(const char *warningString)
{
}

void DebugDrawer::draw3dText(const btVector3 &location, const char *textString)
{
}

void DebugDrawer::setDebugMode(int debugMode)
{
}

int DebugDrawer::getDebugMode() const
{
  return btIDebugDraw::DBG_DrawWireframe;
}
} // namespace Renderer

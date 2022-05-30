#include <Renderer/DebugDrawer.h>

namespace Renderer
{
void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
    mMesh.mVertices.BufferPushBack(from.x());
    mMesh.mVertices.BufferPushBack(from.y());
    mMesh.mVertices.BufferPushBack(from.z());

    mMesh.mVertices.BufferPushBack(to.x());
    mMesh.mVertices.BufferPushBack(to.y());
    mMesh.mVertices.BufferPushBack(to.z());

    mMesh.mIndices.push_back(mMesh.mIndices.size());
    mMesh.mIndices.push_back(mMesh.mIndices.size());
}

void DebugDrawer::Draw(const glm::mat4 &camera, const glm::mat4 &projection)
{
    // mBackend->ClearCommandQueue();
    if (mMesh.mVertices.IsEmpty()) {
        return;
    }
#if 0
  auto handle = mBackend->SubmitMesh(&mMesh);
  std::vector<ShaderData> shaderData({ShaderData("color", {1.0, 1.0, 1.0}),
                                      ShaderData("camera", camera),
                                      ShaderData("projection", projection)});
  DrawCommand dc(CommandType::DrawLine, handle, shaderData);
  mBackend->SubmitCommand(dc);
  mBackend->Draw();
  mBackend->RemoveMesh(handle);
  mMesh.Clear();
#endif
}

void DebugDrawer::drawContactPoint(
    const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
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

#include "DebugRenderer.h"

#include <Renderer/Renderer.h>

void DebugRenderer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
  mLineData.push_back(from.x());
  mLineData.push_back(from.y());
  mLineData.push_back(from.z());
  mLineData.push_back(to.x());
  mLineData.push_back(to.y());
  mLineData.push_back(to.z());
  mIBData.push_back(mIBData.size());
  mIBData.push_back(mIBData.size());
}

void DebugRenderer::Draw(const glm::mat4 &camera, const glm::mat4 &projection)
{
  if (mLineBuffer == focus::INVALID_HANDLE || mLineBufferByteSize < (mLineData.size() * sizeof(f32))) {
    focus::VertexBufferDescriptor descriptor = {
        .inputDescriptorName = {"vPosition"},
        .bufferType = focus::BufferType::SingleType,
        .types = {focus::VarType::Vec3},
        .sizeInBytes = (u32)(mLineData.size() * sizeof(f32)),
        .elementSizeInBytes = 3 * sizeof(f32),
        .usage = focus::BufferUsage::Static,
    };
    mLineBufferByteSize = (u32)(mLineData.size() * sizeof(f32));
    if (mLineBuffer != focus::INVALID_HANDLE) {
      focus::gContext->DestroyVertexBuffer(mLineBuffer);
      focus::gContext->DestroyIndexBuffer(mIB);
    }
    mLineBuffer = focus::gContext->CreateVertexBuffer(mLineData.data(), mLineBufferByteSize, descriptor);
    // clang-format off
    mIB = focus::gContext->CreateIndexBuffer(mIBData.data(), mIBData.size() * sizeof(u32), {
        .type = focus::IndexBufferType::U32,
        .sizeInBytes = (u32)(mIBData.size() * sizeof(u32)),
        .usage = focus::BufferUsage::Static,
    });
    // clang-format on
  } else {
    focus::gContext->UpdateVertexBuffer(mLineBuffer, mLineData.data(), mLineBufferByteSize);
    focus::gContext->UpdateIndexBuffer(mIB, mIBData.data(), (u32)(mIBData.size() * sizeof(u32)));
  }
  mLineData.clear();
  mIBData.clear();
  // clang-format off
  LineConstants constants = {.mvp = projection * camera, .color = glm::vec3(1.0, 0.0, 0.0)};
  focus::gContext->UpdateConstantBuffer(mLineConstants, (void*)&constants, sizeof(LineConstants));
  focus::gContext->Draw(focus::Primitive::Lines, {}, mLineShader, {
      .vbHandles = {mLineBuffer},
      .cbHandles = {mLineConstants},
      .ibHandle = mIB,
      .indexed = true,
  });
  // clang-format on
}

void DebugRenderer::drawContactPoint(
    const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
{
}
void DebugRenderer::reportErrorWarning(const char *warningString)
{
}
void DebugRenderer::draw3dText(const btVector3 &location, const char *textString)
{
}
void DebugRenderer::setDebugMode(int debugMode)
{
}

int DebugRenderer::getDebugMode() const
{
  return btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_NoDeactivation;
}

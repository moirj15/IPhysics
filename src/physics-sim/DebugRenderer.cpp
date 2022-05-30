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
    // TODO: look into adding an Update() for normal VertexBuffers
    if (mLineBuffer == focus::DynamicVertexBuffer::Invalid()
        || mLineBufferByteSize < (mLineData.size() * sizeof(f32))) {
        mLineBufferByteSize = (u32)(mLineData.size() * sizeof(f32));
        if (mLineBuffer != focus::DynamicVertexBuffer::Invalid()) {
            m_device->DestroyDynamicVertexBuffer(mLineBuffer);
            m_device->DestroyDynamicIndexBuffer(mIB);
        }
        mLineBuffer = m_device->CreateDynamicVertexBuffer(vb_layout, mLineData.data(), mLineBufferByteSize);
        mIB = m_device->CreateDynamicIndexBuffer(ib_layout, mIBData.data(), mIBData.size() * sizeof(u32));
        m_line_scene_state = focus::SceneState{
            .dynamic_vb_handles = {mLineBuffer},
            .dynamic_ib_handle = mIB,
            .indexed = true,
        };
    } else {
        m_device->UpdateDynamicVertexBuffer(mLineBuffer, mLineData.data(), mLineBufferByteSize, 0);
        m_device->UpdateDynamicIndexBuffer(mIB, mIBData.data(), (u32)(mIBData.size() * sizeof(u32)), 0);
    }
    LineConstants constants = {.mvp = projection * camera, .color = glm::vec3(1.0, 0.0, 0.0)};
    m_device->UpdateConstantBuffer(mLineConstants, reinterpret_cast<void *>(&constants), sizeof(LineConstants), 0);

    m_device->BeginPass("Line Pass");
    m_device->BindSceneState(m_line_scene_state);
    m_device->BindPipeline(m_line_pipeline);

    m_device->Draw(focus::Primitive::Lines, 0, mIBData.size() / 2);
    m_device->EndPass();
    mLineData.clear();
    mIBData.clear();
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

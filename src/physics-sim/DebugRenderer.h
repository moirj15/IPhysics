#pragma once

#include <Common.h>
#include <LinearMath/btIDebugDraw.h>
#include <focus.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

class DebugRenderer : public btIDebugDraw
{
    focus::Device *m_device;
    // TODO: switch to dynamic buffers
    focus::DynamicVertexBuffer mLineBuffer;
    focus::DynamicIndexBuffer mIB;
    focus::ConstantBuffer mLineConstants;
    focus::Shader mLineShader;
    u32 mLineBufferByteSize = 0;
    std::vector<f32> mLineData;
    std::vector<u32> mIBData;
    struct LineConstants {
        glm::mat4 mvp;
        glm::vec3 color;
    };
    focus::ConstantBufferLayout cb_layout;
    focus::VertexBufferLayout vb_layout;
    focus::IndexBufferLayout ib_layout;

    // TODO: the line rendering should be pulled out
    focus::Pipeline m_line_pipeline;
    focus::SceneState m_line_scene_state;

  public:
    explicit DebugRenderer(focus::Device *device) :
        m_device(device), mLineShader(m_device->CreateShaderFromSource(
                              "Line", ReadFile("shaders/line.vert"), ReadFile("shaders/line.frag"))),
        cb_layout("constants"), vb_layout("line_buffer"),
        ib_layout(focus::IndexBufferType::U32, focus::BufferUsage::Dynamic)
    {
        m_line_pipeline = m_device->CreatePipeline({.shader = mLineShader});
        cb_layout.Add("mvp", focus::VarType::Float4x4).Add("color", focus::VarType::Float3);
        mLineConstants = device->CreateConstantBuffer(cb_layout, nullptr, sizeof(LineConstants));

        vb_layout.Add("vPosition", focus::VarType::Float3);
    }
    void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;

    void Draw(const glm::mat4 &camera, const glm::mat4 &projection);
    void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime,
        const btVector3 &color) override;
    void reportErrorWarning(const char *warningString) override;
    void draw3dText(const btVector3 &location, const char *textString) override;
    void setDebugMode(int debugMode) override;
    int getDebugMode() const override;
};

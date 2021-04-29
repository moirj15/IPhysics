#pragma once

#include <Common.h>
#include <Context.hpp>
#include <LinearMath/btIDebugDraw.h>
#include <glm/mat4x4.hpp>
#include <vector>

class DebugRenderer : public btIDebugDraw
{
  focus::VertexBufferHandle mLineBuffer = focus::INVALID_HANDLE;
  focus::IndexBufferHandle mIB;
  focus::ConstantBufferHandle mLineConstants;
  focus::ShaderHandle mLineShader;
  u32 mLineBufferByteSize = 0;
  std::vector<f32> mLineData;
  std::vector<u32> mIBData;
  struct LineConstants {
    glm::mat4 mvp;
    glm::vec3 color;
  };

public:
  DebugRenderer() :
      mLineShader(focus::gContext->CreateShaderFromSource(
          "Line", ReadFile("shaders/line.vert"), ReadFile("shaders/line.frag"))),
      mLineConstants(focus::gContext->CreateConstantBuffer(nullptr, sizeof(LineConstants),
          {
              .name = "constants",
              .types = {focus::VarType::Mat4, focus::VarType::Vec3},
              .slot = 0,
              .usage = focus::BufferUsage::Static,
              .sizeInBytes = sizeof(LineConstants),
          }))
  {
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

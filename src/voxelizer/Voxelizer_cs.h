#pragma once

#include "Objects.h"
#include <Context.hpp>
// TODO: need to move the "shared library" stuff into different place
#include <Common.h>

namespace iphys
{


class Voxelizer
{
  // TODO: maybe useful for general buffer management
  struct Buffer {
    focus::BufferHandle handle = focus::INVALID_HANDLE;
    const focus::ShaderBufferDescriptor descriptor;
  };

  /// Structure that will be used to populate the constant buffer for the shader
  struct ConstantBuffer {
    /// The minimum point of the bounding box
    glm::vec3 min;
    float pad0;
    /// The maximum point of the bounding box
    glm::vec3 max;
    float pad1;
    // TODO: better doc here
    /// The voxel dimensions that we want to voxelize to
    glm::ivec3 voxelDimensions;
    /// The size of each individual voxel
    float voxelSize;
  };

  focus::ShaderHandle mHullPass = focus::INVALID_HANDLE;
  focus::ConstantBufferDescriptor mConstantDescriptor = {
      .name = "Constants",
      .slot = 0,
  };

  focus::ConstantBufferHandle mHullPassConstantBuffer = focus::INVALID_HANDLE;
  Buffer mHullPassInputData = {
      .descriptor = {
          .name = "InputData",
          .slot = 0,
          .accessMode = focus::AccessMode::ReadOnly,
          .types = {focus::VarType::Vec3},
      },
  };
  Buffer mHullPassVertexOutput = {
      .descriptor = {
          .name = "vertexOwner",
          .slot = 0,
          .accessMode = focus::AccessMode::WriteOnly,
          .types = {focus::VarType::Int},
      },
  };
  Buffer mHullPassVoxelOutput = {
      .descriptor = {
          .name = "existingVoxels",
          .slot = 1,
          .accessMode = focus::AccessMode::WriteOnly,
          .types = {focus::VarType::UInt},
      },
  };

public:
  Voxelizer() :
      mHullPass(focus::gContext->CreateComputeShaderFromSource(
          "HullPass", std::get<0>(ReadFile("shaders/HullPassCS.hlsl")))),
      mHullPassConstantBuffer(focus::gContext->CreateConstantBuffer(nullptr, 48, mConstantDescriptor))
  {
  }
  ~Voxelizer()
  {
    focus::gContext->DestroyConstantBuffer(mHullPassConstantBuffer);
    focus::gContext->DestroyShaderBuffer(mHullPassInputData.handle);
    focus::gContext->DestroyShaderBuffer(mHullPassVertexOutput.handle);
    focus::gContext->DestroyShaderBuffer(mHullPassVoxelOutput.handle);
  }

  objs::VoxelMesh Voxelize(const objs::AABB &meshAABB, f32 voxelSize, const objs::Mesh &mesh);
};

} // namespace iphys
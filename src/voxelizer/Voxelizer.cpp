#include "Voxelizer.h"

namespace iphys
{
// glm::ivec3 voxelDimmensions = (maxP + (-minP)) / (1.0f / params.mVoxelSize);
//// TODO: need a buffer class
//
// mHullPassInputData = focus::gContext->CreateShaderBuffer(
//    (void *)mMesh.mVertices.BufferPtr(), mMesh.mVertices.BufferSizeBytes(), inputDesc);
//
// mHullPassVertexOutput =
// focus::gContext->CreateShaderBuffer(nullptr, sizeof(f32) * mMesh.mVertices.BufferSize(), vertexOutputDesc);
// std::vector<u32> voxelPassZeroed(8 * sizeof(u32));
//// TODO: hard-coded temp
// mHullPassVoxelOutput = focus::gContext->CreateShaderBuffer(voxelPassZeroed.data(), 8 * sizeof(u32), voxelOutputDesc);
//}
// focus::ComputeState computeState = {
//    .bufferHandles = {mHullPassInputData, mHullPassVertexOutput, mHullPassVoxelOutput},
//    .cbHandles = {mHullPassConstantBuffer},
//};
// focus::gContext->DispatchCompute(mMesh.mVertices.BufferSize() / 3, 1, 1, mHullPass, computeState);
// focus::gContext->WaitForMemory(0);
//
// s32 *voxelOutput = (s32 *)focus::gContext->MapBufferPtr(mHullPassVoxelOutput, focus::AccessMode::ReadOnly);
// focus::gContext->UnmapBufferPtr(mHullPassVoxelOutput);

objs::VoxelMesh Voxelizer::Voxelize(const objs::AABB &meshAABB, f32 voxelSize, const objs::Mesh &mesh)
{
  glm::vec3 voxelDimensions = (meshAABB.max + (-meshAABB.min)) / (1.0f / voxelSize);
  ConstantBuffer cb = {
      .min = meshAABB.min,
      .max = meshAABB.max,
      .voxelDimensions = voxelDimensions,
      .voxelSize = voxelSize,
  };
  objs::VoxelMesh ret = {
      .dimensionsVoxelSpace = voxelDimensions,
      .dimensionsObjectSpace = meshAABB.max + -meshAABB.min,
      .initialVoxelSize = glm::vec3(voxelSize),
  };
  focus::gContext->UpdateConstantBuffer(mHullPassConstantBuffer, (void *)&cb, sizeof(ConstantBuffer));
  focus::gContext->UpdateShaderBuffer(
      mHullPassInputData.handle, (void *)mesh.vertices.data(), mesh.vertices.size() * sizeof(f32));
  u32 voxelOutputSize = voxelDimensions.x * voxelDimensions.y * voxelDimensions.z;
  focus::gContext->UpdateShaderBuffer(mHullPassVoxelOutput.handle, nullptr, voxelOutputSize * sizeof(u32));
  focus::gContext->UpdateShaderBuffer(mHullPassVertexOutput.handle, nullptr, mesh.vertices.size() * sizeof(f32));

  focus::ComputeState computeState = {
      .bufferHandles = {mHullPassInputData.handle, mHullPassVertexOutput.handle, mHullPassVoxelOutput.handle},
      .cbHandles = {mHullPassConstantBuffer},
  };
  focus::gContext->DispatchCompute(mesh.vertices.size() / 3, 1, 1, mHullPass, computeState);

  // Retrieve the voxel data from the gpu
  bool *voxelExistance =
      (bool *)focus::gContext->MapBufferPtr(mHullPassVoxelOutput.handle, focus::AccessMode::ReadOnly);
  for (u32 x = 0; x < (u32)voxelDimensions.x; x++) {
    for (u32 y = 0; y < (u32)voxelDimensions.y; y++) {
      for (u32 z = 0; z < (u32)voxelDimensions.z; z++) {
        objs::Voxel voxel;
        ret.voxels.emplace(glm::uvec3(x, y, z), voxel);
      }
    }
  }
  focus::gContext->UnmapBufferPtr(mHullPassVoxelOutput.handle);

  return ret;
}

} // namespace iphys

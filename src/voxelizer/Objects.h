#pragma once
#include <Common.h>
#include <vector>
#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>
namespace objs {


struct Voxel
{
  glm::vec3 size;
  glm::vec3 mDimensions{0.0f};
  glm::vec3 mPosition{0.0f};
  glm::vec3 mPositionRelativeToCenter{0.0f};
  glm::ivec3 mNeighbors[6];
  std::vector<u32> mMeshVertices;
  // TODO
//  std::vector<BezierCurve> mBezierCurves;
};

// TODO: add helpers and move to a better file
struct Mesh {
  std::vector<f32> vertices;
  std::vector<u32> indices;
  std::vector<f32> textureCoordinates;
  inline glm::vec3 GetVertex(u32 i) {
    assert(i < vertices.size() / 3);
    return glm::vec3(vertices[i * 3], vertices[(i * 3) + 1], vertices[(i * 3) + 2]);
  }
  inline void AddVertex(const glm::vec3 &v) {
    vertices.push_back(v[0]);
    vertices.push_back(v[1]);
    vertices.push_back(v[2]);
  }
};

struct VoxelMesh {
  /// The dimensions of the voxelized object in discrete voxel space
  glm::uvec3 dimensionsVoxelSpace;
  /// The dimensions of the voxelized object in continuous object space
  glm::vec3 dimensionsObjectSpace;
  /// The initial size of the voxels, used for resetting the simulation
  glm::vec3 initialVoxelSize;
  /// The hash-map of the voxels that make up our voxelized object
  std::unordered_map<glm::uvec3, Voxel> voxels;
};

struct AABB
{
  glm::vec3 min = glm::vec3((float)INFINITY, (float)INFINITY, (float)INFINITY);
  glm::vec3 max = glm::vec3(-INFINITY, -INFINITY, -INFINITY);
};
}
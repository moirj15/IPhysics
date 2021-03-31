#pragma once
#include <Common.h>
#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <vector>
namespace objs
{

struct Voxel {
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
  //std::vector<f32> textureCoordinates;
  std::vector<f32> normals;

  std::vector<u32> indices;
  inline glm::vec3 GetVertex(u32 i)
  {
    assert(i < vertices.size() / 3);
    return glm::vec3(vertices[i * 3], vertices[(i * 3) + 1], vertices[(i * 3) + 2]);
  }
  inline glm::vec3 GetNormal(u32 i)
  {
    assert(i < normals.size() / 3);
    return glm::vec3(normals[i * 3], normals[(i * 3) + 1], normals[(i * 3) + 2]);
  }
  inline void AddVertex(const glm::vec3 &v)
  {
    vertices.push_back(v[0]);
    vertices.push_back(v[1]);
    vertices.push_back(v[2]);
  }
  inline void SetVertex(u32 i, const glm::vec3 &v) {
    SetVec3(&vertices, i, v);
  }
  inline void SetNormal(u32 i, const glm::vec3 &n) {
    SetVec3(&normals, i, n);
  }
  inline u32 VerticesSizeInBytes() { return vertices.size() * sizeof(f32); }
  inline u32 IndicesSizeInBytes() { return indices.size() * sizeof(u32); }
  inline std::vector<f32> GetInterleved() {
    std::vector<f32> ret;
    for (u32 i = 0; i < (vertices.size() / 3); i++) {
      auto v = GetVertex(i);
      auto n = GetNormal(i);
      ret.push_back(v.x);
      ret.push_back(v.y);
      ret.push_back(v.z);

      ret.push_back(n.x);
      ret.push_back(n.y);
      ret.push_back(n.z);
    }
    return ret;
  }
private:
  inline void SetVec3(std::vector<f32> *data, u32 i, const glm::vec3 &v) {
    (*data)[i * 3] = v.x;
    (*data)[(i * 3) + 1] = v.x;
    (*data)[(i * 3) + 1] = v.x;
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

struct AABB {
  glm::vec3 min = glm::vec3((float)INFINITY, (float)INFINITY, (float)INFINITY);
  glm::vec3 max = glm::vec3(-INFINITY, -INFINITY, -INFINITY);
};
} // namespace objs
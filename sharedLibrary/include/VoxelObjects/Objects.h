#pragma once
#include <Common.h>
#include <array>
#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <vector>
namespace objs
{
struct BezierCurve {
  std::vector<glm::vec3> controlPoints;
  // The value of t where the line starts
  f32 firstT = 0.0f;
  // The value of t where the line ends
  f32 secondT = 1.0f;

  std::vector<u32> effectedPoints;
#if 0
  //BezierCurve() = default;

  ///**
  // * \brief: Constructor for the BezierCurve.
  // *
  // * \param controlPoints: The control points of the bezier curves. This array will contain
  // * either 3 or 4 points. If the size is 3, then we'll assume that the first control point lies on
  // * the edge that intersects the voxel hull and that the last control point results from
  // * casting a ray to the opposite voxel hull. If the size is 4, then the starting and final
  // * control points come from ray casts. The points that aren't the starting or final control points
  // * come from the vertices of the edge that this BezierCurve will represent.
  // */
  //BezierCurve(const std::vector<glm::vec3> &controlPoints, const std::vector<u32> &effectedPoints) :
  //    controlPoints(controlPoints), effectedPoints(effectedPoints)
  //{
  //  assert((controlPoints.size() == 3) || (controlPoints.size() == 4));
  //  if (controlPoints.size() == 3)
  //  {
  //    CalculateQuadraticTValue();
  //  }
  //  else
  //  {
  //    CalculateCubicTValues();
  //  }
  //}

  //BezierCurve(const BezierCurve &b) :
  //    controlPoints(b.controlPoints), firstT(b.firstT), secondT(b.secondT),
  //    effectedPoints(b.effectedPoints)
  //{
  //}

  // TODO: Cleanup
  void CalculateQuadraticTValue()
  {
    auto p = controlPoints;
    auto a = p[0] - 2.0f * p[1] + p[2];
    auto b = -2.0f * p[0] + 2.0f * p[1];
    auto c = -p[1] + p[0];
    glm::vec3 posT(0.0f);
    glm::vec3 negT(0.0f);
    for (u32 i = 0; i < 3; i++)
    {
      std::complex<f32> ca(a[i]);
      std::complex<f32> cb(b[i]);
      std::complex<f32> cc(c[i]);
      posT[i] = ((-cb + sqrt(pow(cb, 2.0f) - 4.0f * ca * cc)) / (2.0f * ca)).real();
      negT[i] = ((-cb - sqrt(pow(cb, 2.0f) - 4.0f * ca * cc)) / (2.0f * ca)).real();
    }

    f32 bestDistance = INFINITY;
    f32 best = 1.0f; // posT.x;

    for (u32 i = 0; i < 3; i++)
    {

      if (posT[i] <= 1.0f && posT[i] >= 0.0f
          && glm::length(EvaluateQuadraticCurve(posT[i]) - controlPoints[1]) < bestDistance)
      {
        bestDistance = glm::length(EvaluateQuadraticCurve(posT[i]) - controlPoints[1]);
        best = posT[i];
      }
      if (negT[i] <= 1.0f && negT[i] >= 0.0f
          && glm::length(EvaluateQuadraticCurve(negT[i]) - controlPoints[1]) < bestDistance)
      {
        bestDistance = glm::length(EvaluateQuadraticCurve(negT[i]) - controlPoints[1]);
        best = posT[i];
      }
    }
    assert(!isnan(best) || !isnan(-best));
    firstT = best;
  }

  void CalculateCubicTValues()
  {
    // TODO: really clean this up
    auto FindBestTValue = [this](u64 index) {
      auto points = controlPoints;
      f32 bestDistance = INFINITY;
      f32 bestT = 0.0f;
      for (u32 i = 0; i < 3; i++)
      {

        std::complex<f32> a =
            -points[0][i] + (3.0f * points[1][i]) - (3.0f * points[2][i]) + points[3][i];
        std::complex<f32> b = 3.0f * points[0][i] - 6.0f * points[1][i] + 3.0f * points[2][i];
        std::complex<f32> c = -3.0f * points[0][i] + 3.0f * points[1][i];
        std::complex<f32> d = points[0][i] - points[index][i];

        auto p = -b / (3.0f * a);
        auto q = pow(p, 3.0f) + (b * c - 3.0f * a * d) / (6.0f * pow(a, 2.0f));
        auto r = c / (3.0f * a);

        auto t = pow(q + pow(q * q + pow(r - (p * p), 3.0f), 1.0f / 2.0f), 1.0f / 3.0f)
                 + pow(q - pow((q * q) + pow(r - (p * p), 3.0f), 1.0f / 2.0f), 1.0f / 3.0f) + p;
        if (glm::length(EvaluateCubicCurve(t.real()) - points[index]) < bestDistance)
        {
          bestDistance = glm::length(EvaluateCubicCurve(t.real()) - points[index]);
          bestT = t.real();
        }
      }
      assert(!isnan(bestT) || !isnan(-bestT));
      return bestT;
    };

    firstT = FindBestTValue(1);
    secondT = FindBestTValue(2);
  }

  inline glm::vec3 EvaluateQuadraticCurve(f32 t)
  {
    return controlPoints[1] + powf(1.0f - t, 2.0f) * (controlPoints[0] - controlPoints[1])
           + powf(t, 2.0f) * (controlPoints[2] - controlPoints[1]);
    //     auto q0 = glm::mix(points[0], points[1], t);
    //     auto q1 = glm::mix(points[1], points[2], t);
    //     return glm::mix(q0, q1, t);
  }
  inline glm::vec3 EvaluateCubicCurve(f32 t)
  {
    return powf(1.0f - t, 3) * controlPoints[0] + 3.0f * powf(1.0f - t, 2) * t * controlPoints[1]
           + 3.0f * (1 - t) * powf(t, 2.0) * controlPoints[2] + powf(t, 3) * controlPoints[3];
    //     auto q0 = glm::mix(points[0], points[1], t);
    //     auto q1 = glm::mix(points[1], points[2], t);
    //     auto q2 = glm::mix(points[2], points[3], t);
    //     auto r0 = glm::mix(q0, q1, t);
    //     auto r1 = glm::mix(q1, q2, t);
    //     return glm::mix(r0, r1, t);
  }
#endif
};

struct Voxel {
  glm::vec3 size;
  glm::vec3 dimmensions{0.0f};
  glm::vec3 position{0.0f};
  glm::vec3 positionRelativeToCenter{0.0f};
  glm::vec3 relativePositionDelta{0.0f};

  // 0 left, 1 right, 2 bottom, 3 top, 4 front, 5 back
  std::array<bool, 6> neighbors;

  std::vector<u32> meshVertices;
  std::vector<BezierCurve> bezierCurves;

  static constexpr glm::ivec3 NEIGHBOR_OFFSETS[] = {
      glm::vec3(-1, 0, 0), // left
      glm::vec3(1, 0, 0),  // right

      glm::vec3(0, -1, 0), // bottom
      glm::vec3(0, 1, 0),  // top

      glm::vec3(0, 0, -1), // front
      glm::vec3(0, 0, 1),  // back
  };

  NODISCARD inline bool InVoxel(const u32 vert) const
  {
    for (u32 v : meshVertices) {
      if (v == vert) {
        return true;
      }
    }
    return false;
  }
  // TODO: clean-up
  inline glm::vec3 CalculateFrom3Points(const std::vector<glm::vec3> &points, f32 t)
  {
    return points[1] + powf(1.0f - t, 2.0f) * (points[0] - points[1])
           + powf(t, 2.0f) * (points[2] - points[1]);
    //     auto q0 = glm::mix(points[0], points[1], t);
    //     auto q1 = glm::mix(points[1], points[2], t);
    //     return glm::mix(q0, q1, t);
  }
  inline glm::vec3 CalculateFrom4Points(const std::vector<glm::vec3> &points, f32 t)
  {
    return powf(1.0f - t, 3) * points[0] + 3.0f * powf(1.0f - t, 2) * t * points[1]
           + 3.0f * (1 - t) * powf(t, 2.0) * points[2] + powf(t, 3) * points[3];
    //     auto q0 = glm::mix(points[0], points[1], t);
    //     auto q1 = glm::mix(points[1], points[2], t);
    //     auto q2 = glm::mix(points[2], points[3], t);
    //     auto r0 = glm::mix(q0, q1, t);
    //     auto r1 = glm::mix(q1, q2, t);
    //     return glm::mix(r0, r1, t);
  }
};

// TODO: add helpers and move to a better file
struct Mesh {
  std::vector<f32> vertices;
  // std::vector<f32> textureCoordinates;
  std::vector<f32> normals;

  std::vector<u32> indices;
  inline glm::vec3 GetVertex(u32 i) const
  {
    assert(i < vertices.size() / 3);
    return glm::vec3(vertices[i * 3], vertices[(i * 3) + 1], vertices[(i * 3) + 2]);
  }
  inline glm::vec3 GetNormal(u32 i) const
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
  inline void SetVertex(u32 i, const glm::vec3 &v) { SetVec3(&vertices, i, v); }
  inline void SetNormal(u32 i, const glm::vec3 &n) { SetVec3(&normals, i, n); }
  inline u32 VerticesSizeInBytes() const { return (u32)vertices.size() * sizeof(f32); }
  inline u32 IndicesSizeInBytes() const { return (u32)indices.size() * sizeof(u32); }

  // TODO: only create buffer once and update
  /**
   * @brief Creates a vector of interleaved vertex data (position and normal)
   * @return The vector of interleaved vertex data
   */
  inline std::vector<f32> GetInterleved() const
  {
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
  inline void SetVec3(std::vector<f32> *data, u32 i, const glm::vec3 &v)
  {
    (*data)[i * 3] = v.x;
    (*data)[(i * 3) + 1] = v.y;
    (*data)[(i * 3) + 2] = v.z;
  }
};

struct VoxelMesh {
  /// The dimensions of the voxelized object in discrete voxel space
  glm::uvec3 dimensionsVoxelSpace;
  /// The dimensions of the voxelized object in continuous object space
  glm::vec3 dimensionsObjectSpace;
  /// The initial size of the voxels, used for resetting the simulation
  f32 initialVoxelSize;
  /// The hash-map of the voxels that make up our voxelized object
  std::unordered_map<glm::uvec3, Voxel> voxels;
};

struct AABB {
  glm::vec3 min = glm::vec3((float)INFINITY, (float)INFINITY, (float)INFINITY);
  glm::vec3 max = glm::vec3(-INFINITY, -INFINITY, -INFINITY);
};

} // namespace objs
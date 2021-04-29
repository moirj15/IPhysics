#pragma once

#include <Common.h>

#ifndef GLM_ENABLE_EXPERIMENTAL
  #define GLM_ENABLE_EXPERIMENTAL
#endif
#include "BezierCurve.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace VoxObj
{

#if 0
struct Voxel
{
  glm::vec3 mDimensions{0.0f};
  glm::vec3 mPosition{0.0f};
  glm::vec3 mPositionRelativeToCenter{0.0f};
  glm::vec3 mRelativePositionDelta{0.0f};
  std::vector<glm::uvec3> mNeighbors;
  std::vector<u32> mMeshVertices;
  std::vector<BezierCurve> mBezierCurves;

  Voxel() = default;

  explicit Voxel(const f32 dimmensions, const glm::vec3 position) :
      mDimensions(dimmensions), mPosition(position)
  {
  }

  Voxel(const Voxel &v) :
      mDimensions(v.mDimensions), mPosition(v.mPosition), mNeighbors(v.mNeighbors),
      mMeshVertices(v.mMeshVertices), mBezierCurves(v.mBezierCurves)
  {
  }

  inline void UpdateBezierCurves(const glm::vec3 &toNewPosition)
  {
    for (u32 i = 0; i < mBezierCurves.size(); i++)
    {
      for (u32 b = 0; b < mBezierCurves[i].mControlPoints.size(); b++)
      {
        auto &controlPoints = mBezierCurves[i].mControlPoints;
        controlPoints[b] += toNewPosition;
      }
    }
  }

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

  NODISCARD inline bool InVoxel(const u32 vert) const
  {
    for (u32 v : mMeshVertices)
    {
      if (v == vert)
      {
        return true;
      }
    }
    return false;
  }
};
#endif

} // namespace VoxObj
#pragma once
#include "../common.h"

#include <assert.h>
#include <glm/vec3.hpp>
#include <vector>

struct BezierCurve
{
  std::vector<glm::vec3> mControlPoints;
  // The value of t where the line starts
  f32 mTStart = 0.0f;
  // The value of t where the line ends
  f32 mTEnd = 1.0f;

  BezierCurve() = default;

  /**
   * \brief: Constructor for the BezierCurve.
   *
   * \param controlPoints: The control points of the bezier curves. This array will contain
   * either 3 or 4 points. If the size is 3, then we'll assume that the first control point lies on
   * the edge that intersects the voxel hull and that the last control point results from
   * casting a ray to the opposite voxel hull. If the size is 4, then the starting and final
   * control points come from ray casts. The points that aren't the starting or final control points
   * come from the vertices of the edge that this BezierCurve will represent.
   */
  BezierCurve(const std::vector<glm::vec3> &controlPoints) : mControlPoints(controlPoints)
  {
    assert((mControlPoints.size() == 3) || (mControlPoints.size() == 4));
    f32 totalLength = glm::length(controlPoints.back() - controlPoints.front());
    if (totalLength == 0.0f)
    {
      totalLength = 1.0f;
    }
    if (mControlPoints.size() == 3)
    {
      mTStart = glm::length(controlPoints[1] - controlPoints[0]) / totalLength;
      mTEnd = glm::length(controlPoints[1] - controlPoints[2]) / totalLength;
    }
    else
    {
      mTStart = glm::length(controlPoints[1] - controlPoints[0]) / totalLength;
      mTEnd = glm::length(controlPoints[2] - controlPoints[3]) / totalLength;
    }
    if (mTEnd > 1.0f)
    {
      mTEnd = 1.0f;
    }
    if (mTStart < 0.0f)
    {
      mTStart = 0.0f;
    }
    assert(!isnan(mTStart) && !isnan(mTStart) && "NAN");
  }

  BezierCurve(const BezierCurve &b) :
      mControlPoints(b.mControlPoints), mTStart(b.mTStart), mTEnd(b.mTEnd)
  {
  }
};
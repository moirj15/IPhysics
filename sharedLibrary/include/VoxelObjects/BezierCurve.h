#pragma once
#include "../../Common.h"

#include <assert.h>
#include <complex>
#include <glm/vec3.hpp>
#include <vector>

struct BezierCurve
{
  std::vector<glm::vec3> mControlPoints;
  // The value of t where the line starts
  f32 mFirstT = 0.0f;
  // The value of t where the line ends
  f32 mSecondT = 1.0f;

  std::vector<u32> mEffectedPoints;

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
  BezierCurve(const std::vector<glm::vec3> &controlPoints, const std::vector<u32> &effectedPoints) :
      mControlPoints(controlPoints), mEffectedPoints(effectedPoints)
  {
    assert((mControlPoints.size() == 3) || (mControlPoints.size() == 4));
    if (mControlPoints.size() == 3)
    {
      CalculateQuadraticTValue();
    }
    else
    {
      CalculateCubicTValues();
    }
  }

  BezierCurve(const BezierCurve &b) :
      mControlPoints(b.mControlPoints), mFirstT(b.mFirstT), mSecondT(b.mSecondT),
      mEffectedPoints(b.mEffectedPoints)
  {
  }

  // TODO: Cleanup
  void CalculateQuadraticTValue()
  {
    auto p = mControlPoints;
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
          && glm::length(EvaluateQuadraticCurve(posT[i]) - mControlPoints[1]) < bestDistance)
      {
        bestDistance = glm::length(EvaluateQuadraticCurve(posT[i]) - mControlPoints[1]);
        best = posT[i];
      }
      if (negT[i] <= 1.0f && negT[i] >= 0.0f
          && glm::length(EvaluateQuadraticCurve(negT[i]) - mControlPoints[1]) < bestDistance)
      {
        bestDistance = glm::length(EvaluateQuadraticCurve(negT[i]) - mControlPoints[1]);
        best = posT[i];
      }
    }
    assert(!isnan(best) || !isnan(-best));
    mFirstT = best;
  }

  void CalculateCubicTValues()
  {
    // TODO: really clean this up
    auto FindBestTValue = [this](u64 index) {
      auto points = mControlPoints;
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

    mFirstT = FindBestTValue(1);
    mSecondT = FindBestTValue(2);
  }

  inline glm::vec3 EvaluateQuadraticCurve(f32 t)
  {
    return mControlPoints[1] + powf(1.0f - t, 2.0f) * (mControlPoints[0] - mControlPoints[1])
           + powf(t, 2.0f) * (mControlPoints[2] - mControlPoints[1]);
    //     auto q0 = glm::mix(points[0], points[1], t);
    //     auto q1 = glm::mix(points[1], points[2], t);
    //     return glm::mix(q0, q1, t);
  }
  inline glm::vec3 EvaluateCubicCurve(f32 t)
  {
    return powf(1.0f - t, 3) * mControlPoints[0] + 3.0f * powf(1.0f - t, 2) * t * mControlPoints[1]
           + 3.0f * (1 - t) * powf(t, 2.0) * mControlPoints[2] + powf(t, 3) * mControlPoints[3];
    //     auto q0 = glm::mix(points[0], points[1], t);
    //     auto q1 = glm::mix(points[1], points[2], t);
    //     auto q2 = glm::mix(points[2], points[3], t);
    //     auto r0 = glm::mix(q0, q1, t);
    //     auto r1 = glm::mix(q1, q2, t);
    //     return glm::mix(r0, r1, t);
  }
};
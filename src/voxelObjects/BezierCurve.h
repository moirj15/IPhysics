#pragma once
#include "../common.h"

#include <glm/vec3.hpp>
#include <vector>

struct BezierCurve
{
  std::vector<glm::vec3> mControlPoints;
  // The value of t where the line starts
  f32 mTStart;
  // The value of t where the line ends
  f32 mTEnd;
};
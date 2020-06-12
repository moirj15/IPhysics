#pragma once

#include <common.h>

namespace VoxGen
{

struct Parameters
{
  f32 mVoxelSize;
  bool mHollow;

  Parameters() : mVoxelSize(1.0f), mHollow(false)
  {
  }
};
} // namespace VoxGen

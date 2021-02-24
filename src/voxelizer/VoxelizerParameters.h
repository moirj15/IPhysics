#pragma once

#include <Common.h>


struct Parameters
{
  f32 mVoxelSize;
  bool mHollow;

  Parameters() : mVoxelSize(1.0f), mHollow(false)
  {
  }
};

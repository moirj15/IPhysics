//
// Created by Jimmy on 2/9/2020.
//

#pragma once

#include "../common.h"

#include <optional>
#include <string>

struct Window;

namespace VoxGen
{

class VoxelizerUI
{
public:
  struct Parameters
  {
    f32 mVoxelSize;
    bool mHollow;

    Parameters() : mVoxelSize(1.0f), mHollow(false)
    {
    }
  };

private:
  std::string mMeshFileName;
  std::string mVoxelMeshFileName;
  Parameters mParameters;
  bool mSaveClicked;
  bool mGenerateVoxelsClicked;
  bool mLoadMeshClicked;

public:
  VoxelizerUI() :
      mMeshFileName(256, 0), mVoxelMeshFileName(256, 0), mSaveClicked(false),
      mGenerateVoxelsClicked(false), mLoadMeshClicked(false)
  {
  }
  ~VoxelizerUI();

  // TODO: these two functions should go in a base class
  void Init(Window *window);

  void Update();

  NODISCARD std::optional<std::string> LoadMeshClicked();

  NODISCARD std::optional<std::string> SaveClicked();

  NODISCARD bool GenerateVoxelsClicked();

  NODISCARD inline Parameters GetParameters() const
  {
    return mParameters;
  }
};
} // namespace VoxGen

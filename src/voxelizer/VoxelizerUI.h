//
// Created by Jimmy on 2/9/2020.
//

#pragma once

#include "VoxelizerParameters.h"

#include <Common.h>
#include <optional>
#include <string>
#include <Context.hpp>

//struct Window;


class VoxelizerUI
{
  std::string mMeshFileName;
  std::string mVoxelMeshFileName;
  Parameters mParameters;
  bool mSaveClicked;
  bool mGenerateVoxelsClicked;
  bool mLoadMeshClicked;

public:
  VoxelizerUI() :
      mMeshFileName(256, 0), mVoxelMeshFileName(256, 0), mSaveClicked(false), mGenerateVoxelsClicked(false),
      mLoadMeshClicked(false)
  {
  }
  ~VoxelizerUI();

  // TODO: these two functions should go in a base class
  void Init(const focus::Window &window);

  void Update(const focus::Window &window);

  NODISCARD std::optional<std::string> LoadMeshClicked();

  NODISCARD std::optional<std::string> SaveClicked();

  NODISCARD bool GenerateVoxelsClicked();

  NODISCARD inline Parameters GetParameters() const { return mParameters; }
};

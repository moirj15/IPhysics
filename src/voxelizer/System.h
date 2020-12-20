//
// Created by Jimmy on 2/9/2020.
//

#pragma once
#include <windows.h>
#include <Common.h>
#include <Renderer/Camera.h>
#include <memory>
#include <Window.hpp>
#include "Voxelizer.h"
#include "VoxelManager.h"

struct Mesh;
struct Window;

namespace Renderer
{
class RendererFrontend;
}

namespace VoxObj
{
class VoxelMesh;
}

namespace VoxGen
{
class VoxelizerUI;
class Voxelizer;

class System
{
  Camera mCamera;
  focus::Window mWindow;
//  std::unique_ptr<Window> mWindow;
  std::unique_ptr<VoxelizerUI> mUI;
  //  std::unique_ptr<Renderer::RendererFrontend> mRenderer;
  Voxelizer mVoxelizer;
  Mesh mMesh;
  std::unique_ptr<VoxObj::VoxelMesh> mVoxelMesh;
  const glm::mat4 mProjectionMat;
  VoxelManager mOriginalManager;
  VoxelManager mMutableManager;

  u32 mCurrentMeshHandle;
  u32 mCurrentVoxelMeshHandle;

public:
  explicit System(HINSTANCE hInstance);
  ~System();

  void Run();

private:
  void CollectInput();
  void LoadMesh();
  void GenerateVoxels();
  void Render();
  void SaveVoxels();
};

} // namespace VoxGen

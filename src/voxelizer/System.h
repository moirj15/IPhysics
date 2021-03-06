//
// Created by Jimmy on 2/9/2020.
//

#pragma once
#include <windows.h>
#include <Common.h>
#include <Renderer/Camera.h>
#include <memory>
#include <Window.hpp>

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
  renderer::Window mWindow;
//  std::unique_ptr<Window> mWindow;
  std::unique_ptr<VoxelizerUI> mUI;
  //  std::unique_ptr<Renderer::RendererFrontend> mRenderer;
  std::unique_ptr<Voxelizer> mVoxelizer;
  std::unique_ptr<Mesh> mMesh;
  std::unique_ptr<VoxObj::VoxelMesh> mVoxelMesh;
  const glm::mat4 mProjectionMat;

  u32 mCurrentMeshHandle;
  u32 mCurrentVoxelMeshHandle;

public:
  System(HINSTANCE hInstance);
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

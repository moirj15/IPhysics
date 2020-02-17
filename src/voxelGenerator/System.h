//
// Created by Jimmy on 2/9/2020.
//

#pragma once
#include "../common.h"

#include <memory>

struct Mesh;
struct Window;

namespace Renderer

{
class RendererFrontend;
}

namespace VoxGen
{
class VoxelizerUI;
class Voxelizer;

class System
{
  std::unique_ptr<Window> mWindow;
  std::unique_ptr<VoxelizerUI> mUI;
  std::unique_ptr<Renderer::RendererFrontend> mRenderer;
  std::unique_ptr<Voxelizer> mVoxelizer;
  std::unique_ptr<Mesh> mMesh;
  u32 mCurrentMeshHandle;

public:
  System();
  ~System();

  void Run();
};

} // namespace VoxGen

#pragma once
//#include "VoxelManager.h"
//#include "Voxelizer_old.h"
#include "Objects.h"
#include "VoxelizerUI.h"
#include "Voxelizer.h"

#include <Common.h>
#include <Context.hpp>
#include <Renderer/Camera.h>
#include <glm/mat4x4.hpp>
#include <memory>

//struct Mesh;
//struct Window;

//namespace Renderer
//{
//class RendererFrontend;
//}
//
//namespace VoxObj
//{
//class VoxelMesh;
//}


class System
{
  Camera mCamera;
  focus::Window mWindow;
  //  std::unique_ptr<Window> mWindow;
  std::unique_ptr<VoxelizerUI> mUI;
  //  std::unique_ptr<Renderer::RendererFrontend> mRenderer;
  VoxGen::Voxelizer mVoxelizer;
  objs::Mesh mMesh;
  objs::VoxelMesh mVoxelMesh;
//  std::unique_ptr<VoxObj::VoxelMesh> mVoxelMesh;
  const glm::mat4 mProjectionMat;
//  VoxelManager mOriginalManager;
//  VoxelManager mMutableManager;

  u32 mCurrentMeshHandle;
  u32 mCurrentVoxelMeshHandle;
  bool mModelLoaded = false;

//  focus::ShaderHandle mHullPass;
//  focus::BufferHandle mHullPassConstantBuffer = focus::INVALID_HANDLE;
//  focus::BufferHandle mHullPassInputData = focus::INVALID_HANDLE;
//  focus::BufferHandle mHullPassVertexOutput = focus::INVALID_HANDLE;
//  focus::BufferHandle mHullPassVoxelOutput = focus::INVALID_HANDLE;

public:
  System();
  ~System();

  void Run();

private:
  void CollectInput();
  void LoadMesh();
  void GenerateVoxels();
  void Render();
  void SaveVoxels();
  void DebugDrawVoxels();
};


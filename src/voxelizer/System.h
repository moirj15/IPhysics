#pragma once
//#include "VoxelManager.h"
//#include "Voxelizer_old.h"
#include <VoxelObjects/Objects.h>
#include "VoxelizerUI.h"
#include "Voxelizer.h"

#include <Common.h>
#include <Context.hpp>
#include <Renderer/Camera.h>
#include <Renderer/Renderer.h>
#include <glm/mat4x4.hpp>
#include <memory>
#include <VoxelObjects/MeshManager.h>

class System
{
  Camera mCamera;
  focus::Window mWindow;
  std::unique_ptr<VoxelizerUI> mUI;
  VoxGen::Voxelizer mVoxelizer;
  objs::Mesh mMesh;
  objs::VoxelMesh mVoxelMesh;
  const glm::mat4 mProjectionMat;

  MeshManager mMeshManager;
  shared::Renderer mRenderer;

  MeshHandle mCurrentMesh = 0;
  bool mModelLoaded = false;
  bool mVoxelized = false;
  bool mEnableDebugDraw = false;

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
};


#pragma once
//#include "VoxelManager.h"
//#include "Voxelizer_old.h"
#include "Voxelizer.h"
#include "VoxelizerUI.h"

#include <Common.h>
#include <Renderer/Camera.h>
#include <Renderer/Renderer.h>
#include <SDL.h>
#include <VoxelObjects/MeshManager.h>
#include <VoxelObjects/Objects.h>
#include <focus.hpp>
#include <glm/mat4x4.hpp>
#include <memory>

class System
{
    Camera mCamera;
    focus::Device *m_device;
    focus::Window mWindow;
    std::unique_ptr<VoxelizerUI> mUI;
    VoxGen::Voxelizer mVoxelizer;

    // TODO: remove since the meshmanager takes care of these
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
    void CollectInput(const SDL_Event &e);
    void LoadMesh();
    void GenerateVoxels();
    void Render();
    void SaveVoxels();
};

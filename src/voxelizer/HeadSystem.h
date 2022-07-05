#pragma once
#include "InputSystem.hpp"
#include "VoxelizerUI.h"

#include <Common.h>
#include <memory>

class RenderSystem;
class PhysicsSystem;

class HeadSystem
{
    InputSystem m_input_system;
    std::unique_ptr<RenderSystem> m_render_system;
    std::unique_ptr<PhysicsSystem> m_physics_system;

    // Camera m_camera;
    //  focus::Device *m_device;
    //  focus::Window m_window;
    std::unique_ptr<VoxelizerUI> m_ui;
    // VoxGen::Voxelizer m_voxelizer;

    // TODO: remove since the meshmanager takes care of these
    // objs::Mesh m_mesh;
    // objs::VoxelMesh m_voxel_mesh;
    // const glm::mat4 m_projection_mat;

    // MeshManager m_mesh_manager;
    // shared::Renderer m_renderer;

    // MeshHandle m_current_mesh = 0;
    // bool m_model_loaded = false;
    // bool m_voxelized = false;
    // bool m_enable_debug_draw = false;

public:
    HeadSystem();

    void Run();

private:
    /*
// void CollectInput(const SDL_Event &e);
void LoadMesh();
void GenerateVoxels();
void Render();
void SaveVoxels();
    */
};

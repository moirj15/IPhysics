#include "HeadSystem.h"

#include "../third_party/imgui/backends/imgui_impl_sdl.h"
#include "../third_party/imgui/backends/imgui_impl_win32.h"
#include "UISystem.hpp"

HeadSystem::HeadSystem() :
    // m_camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    //  m_device(focus::Device::Init(focus::RendererAPI::OpenGL)), m_window(m_device->MakeWindow(1980, 1080)),
//    m_ui(new UISystem()) //, m_projection_mat(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f)),
// m_renderer(m_device, &m_mesh_manager)
//    mCurrentMeshHandle(0), mCurrentVoxelMeshHandle(0)
    m_render_system(new RenderSystem(&m_resource_system)), m_physics_system(new PhysicsSystem(&m_resource_system)), m_ui_system(m_render_system->GetWindow(), &m_resource_system)
{
    // m_ui->Init(m_window);
    m_input_system.Subscribe(m_render_system.get());
    m_input_system.Subscribe(m_physics_system.get());
}

void HeadSystem::Run()
{
    while (m_input_system.BuildEvents()) {
        m_ui_system.Update();
        m_render_system->Step(0, m_entities);
        m_physics_system->Step(0, m_entities);
    }
}

/*
// Private;
void HeadSystem::CollectInput(const SDL_Event &e)
{
#if 0
glfwPollEvents();
#endif
auto &io = ImGui::GetIO();
auto *keys = SDL_GetKeyboardState(nullptr);
if (io.WantCaptureKeyboard) {
    return;
}
f32 boost = 1.0f;
auto key = e.key.keysym.sym;
if ((SDL_GetModState() & SDLK_LSHIFT) && !io.WantCaptureKeyboard) {
    boost = 10.0f;
}
if (keys[SDL_SCANCODE_W]) {
    m_camera.Move(glm::vec3(0.0f, 0.0f, 1.0f) * boost * io.DeltaTime);
}
if (keys[SDL_SCANCODE_S]) {
    m_camera.Move(glm::vec3(0.0f, 0.0f, -1.0f) * boost * io.DeltaTime);
}
if (keys[SDL_SCANCODE_A]) {
    m_camera.Move(glm::vec3(-1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
}
if (keys[SDL_SCANCODE_D]) {
    m_camera.Move(glm::vec3(1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
}
if (keys[SDL_SCANCODE_E]) {
    m_camera.Move(glm::vec3(0.0f, 1.0f, 0.0f) * boost * io.DeltaTime);
}
if (keys[SDL_SCANCODE_Q]) {
    m_camera.Move(glm::vec3(0.0f, -1.0f, 0.0f) * boost * io.DeltaTime);
}
if (!io.WantCaptureMouse && io.MouseDown[0]) {
    f32 screenWidth = f32(m_window.width);
    f32 screenHeight = f32(m_window.height);
    glm::vec2 mouseDelta((screenWidth / 2.0f) - io.MousePos.x, (screenHeight / 2.0f) - io.MousePos.y);
    m_camera.Rotate(mouseDelta * io.DeltaTime * 10.0f);
}
}
*/

/*
void HeadSystem::LoadMesh()
{
    auto meshPath = m_ui->LoadMeshClicked();
    if (meshPath) {
        // Load our mesh if we got a good path
        if (std::filesystem::exists(*meshPath)) {
            m_mesh.Clear();
            tinyobj::ObjReader reader;
            reader.ParseFromFile(*meshPath);
            auto attrib = reader.GetAttrib();
            // assuming one shape for now
            auto shape = reader.GetShapes()[0];
            // TODO: calculate a better index buffer
            for (auto indices : shape.mesh.indices) {
                m_mesh.vertices.push_back(attrib.vertices[indices.vertex_index * 3]);
                m_mesh.vertices.push_back(attrib.vertices[(indices.vertex_index * 3) + 1]);
                m_mesh.vertices.push_back(attrib.vertices[(indices.vertex_index * 3) + 2]);

                u32 normalIndex = indices.normal_index != -1 ? indices.normal_index : indices.vertex_index;
                m_mesh.normals.push_back(attrib.normals[normalIndex * 3]);
                m_mesh.normals.push_back(attrib.normals[(normalIndex * 3) + 1]);
                m_mesh.normals.push_back(attrib.normals[(normalIndex * 3) + 2]);

                m_mesh.indices.push_back(m_mesh.indices.size());
            }

            m_current_mesh = m_mesh_manager.AddMesh(m_mesh);
            m_renderer.LoadMesh(m_current_mesh);

            m_model_loaded = true;
            m_voxelized = false;
        } else {
            // TODO: pop-up or something
        }
    }
}

void HeadSystem::GenerateVoxels()
{
    if (m_model_loaded && m_ui->GenerateVoxelsClicked()) {
        auto params = m_ui->GetParameters();
        m_voxelizer.SetParameters(m_ui->GetParameters());
        m_voxel_mesh = m_voxelizer.Voxelize(m_mesh);
        m_mesh_manager.AddVoxelMesh(m_voxel_mesh, m_current_mesh);
        m_voxelized = true;
        m_renderer.LoadDebugMesh(m_current_mesh);
    }
}

void HeadSystem::Render()
{
    m_renderer.ClearScreen();
    if (m_current_mesh != 0) {
        m_renderer.DrawMesh(m_current_mesh, m_camera, glm::identity<glm::mat4>());
    }
    if (m_voxelized && mEnableDebugDraw)
{
    m_renderer.DrawDebugVoxels(m_current_mesh, m_camera, glm::identity<glm::mat4>());
}

m_ui->Update(m_window);
m_renderer.UpdateScreen(m_window);
}

void HeadSystem::SaveVoxels()
{
    auto savePath = m_ui->SaveClicked();
    if (savePath && m_voxelized) {
        auto params = m_ui->GetParameters();
        shared::Serialize(*m_mesh_manager.GetMesh(m_current_mesh), *m_mesh_manager.GetVoxelMesh(m_current_mesh),
            *savePath, "todo", params.mVoxelSize, params.mHollow);
    }
}

*/

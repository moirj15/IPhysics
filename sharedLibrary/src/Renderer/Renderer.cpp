#include <Renderer/Camera.h>
#include <Renderer/Mesh.h>
#include <Renderer/Renderer.h>
#include <Renderer/Shader.h>
#include <Renderer/Window.h>
#include <VoxelObjects/VoxelMesh.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace shared
{
void Renderer::LoadMesh(MeshHandle handle)
{
    auto *mesh = mMeshManager->GetMesh(handle);
    if (!mMeshes.contains(handle)) {
        auto buffer = mesh->GetInterleved();
        const auto vb_layout = focus::VertexBufferLayout("vertex_input")
                                   .Add("vPosition", focus::VarType::Float3)
                                   .Add("vNormal", focus::VarType::Float3);
        auto vbHandle = m_device->CreateVertexBuffer(vb_layout, buffer.data(), buffer.size() * sizeof(f32));

        const focus::IndexBufferLayout ib_layout(focus::IndexBufferType::U32);
        auto ibHandle = m_device->CreateIndexBuffer(ib_layout, mesh->indices.data(), mesh->IndicesSizeInBytes());

        mMeshes[handle] = {vbHandle, ibHandle};
    } else {
        printf("Try to use static buffers instead or mix with dynamic :)\n");
#if 0
        auto [vbHandle, ibHandle] = mMeshes[handle];
        auto buffer = mesh->GetInterleved();

        // TODO: implement Update*Buffer() methods
        m_device->UpdateVertexBuffer(vbHandle, (void *)buffer.data(), buffer.size() * sizeof(f32));
        m_device->UpdateIndexBuffer(ibHandle, (void *)mesh->indices.data(), mesh->IndicesSizeInBytes());
#endif
    }
}

// TODO: it would be better to do this in a compute shader (and pretty straight forward too).
// But other things have a higher priority so this will have to wait
void Renderer::LoadDebugMesh(MeshHandle handle)
{
    auto *voxelMesh = mMeshManager->GetVoxelMesh(handle);
    u32 baseIndices[] = {
        0, 1, // top front left - top back left
        1, 2, // top back left - top back right
        2, 3, // top back right - top front right
        3, 0, // top front right - tob front left

        4, 5, // bottom front left - bottom back left
        5, 6, // bottom back left - bottom back right
        6, 7, // bottom back right - bottom front right
        7, 4, // bottom front right - bottom front left

        0, 4, // top front left - bottom front left
        1, 5, // top back left - bottom back left
        2, 6, // top back right - bottom back right
        3, 7, // top front right - bottom front right
    };

    std::vector<u32> indices;
    std::vector<f32> vertices;
    u32 currentIndex = 0;
    const auto voxelSize = voxelMesh->initialVoxelSize;
    for (const auto &[key, voxel] : voxelMesh->voxels) {
        const glm::vec3 voxelVerts[] = {
            voxel.position + (glm::vec3(1.0f, 1.0f, 1.0f) * (voxelSize / 2.0f)),   // 0 Front
            voxel.position + (glm::vec3(-1.0f, 1.0f, 1.0f) * (voxelSize / 2.0f)),  // 1
            voxel.position + (glm::vec3(-1.0f, -1.0f, 1.0f) * (voxelSize / 2.0f)), // 2
            voxel.position + (glm::vec3(1.0f, -1.0f, 1.0f) * (voxelSize / 2.0f)),  // 3

            voxel.position + (glm::vec3(1.0f, 1.0f, -1.0f) * (voxelSize / 2.0f)),   // 4 Back
            voxel.position + (glm::vec3(-1.0f, 1.0f, -1.0f) * (voxelSize / 2.0f)),  // 5
            voxel.position + (glm::vec3(-1.0f, -1.0f, -1.0f) * (voxelSize / 2.0f)), // 6
            voxel.position + (glm::vec3(1.0f, -1.0f, -1.0f) * (voxelSize / 2.0f)),  // 7
        };
        for (u32 i = 0; i < ArraySize(baseIndices); i++) {
            indices.push_back(baseIndices[i] + currentIndex);
        }
        currentIndex += 8;
        for (u32 i = 0; i < ArraySize(voxelVerts); i++) {
            vertices.push_back(voxelVerts[i].x);
            vertices.push_back(voxelVerts[i].y);
            vertices.push_back(voxelVerts[i].z);
        }
    }

    if (!mDebugMeshes.contains(handle)) {
        const auto vb_layout = focus::VertexBufferLayout("vertex_input")
                                   .Add("vPosition", focus::VarType::Float3)
                                   .Add("vNormal", focus::VarType::Float3);
        auto vbHandle = m_device->CreateVertexBuffer(vb_layout, vertices.data(), vertices.size() * sizeof(f32));

        const focus::IndexBufferLayout ib_layout(focus::IndexBufferType::U32);
        auto ibHandle = m_device->CreateIndexBuffer(ib_layout, indices.data(), indices.size() * sizeof(u32));

        mMeshes[handle] = {vbHandle, ibHandle};
    } else {
        printf("Try to use static buffers instead or mix with dynamic :)\n");
#if 0
        auto [vbHandle, ibHandle] = mDebugMeshes[handle];
        m_device->UpdateVertexBuffer(vbHandle, (void *)vertices.data(), vertices.size() * sizeof(f32));
        m_device->UpdateIndexBuffer(ibHandle, (void *)indices.data(), indices.size() * sizeof(u32));
#endif
    }
}

void Renderer::DrawMesh(MeshHandle handle, const Camera &camera, const glm::mat4 &model)
{
    auto cameraMat = camera.CalculateMatrix();
    auto projection = glm::infinitePerspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f);
    // auto projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    auto modelView = cameraMat * model;
    auto normalMat = glm::transpose(glm::inverse(modelView));
    PhongConstants constants = {
        .camera = cameraMat,
        .mvp = projection * modelView,
        .normalMat = normalMat,
    };
    m_device->UpdateConstantBuffer(mPhongConstants, (void *)&constants, sizeof(PhongConstants), 0);
    const auto &[vbHandle, ibHandle] = mMeshes[handle];

    m_device->BeginPass("Phong Pass");
    m_device->BindSceneState({.vb_handles = {vbHandle},
        .cb_handles = {mPhongConstants, mPhongFragmentConstants},
        .ib_handle = ibHandle,
        .indexed = true});
    m_device->BindPipeline(m_phong_pipeline);

    m_device->Draw(focus::Primitive::Triangles, 0, mMeshManager->GetMesh(handle)->indices.size() / 3);

    m_device->EndPass();
}

void Renderer::DrawDebugVoxels(MeshHandle handle, const Camera &camera, const glm::mat4 &model)
{
    auto cameraMat = camera.CalculateMatrix();
    auto projection = glm::infinitePerspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f);
    // auto projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    auto modelView = cameraMat * model;
    LineConstants constants = {
        .mvp = projection * modelView,
        .color = glm::vec3(1.0, 0.0, 0.0),
    };
    m_device->UpdateConstantBuffer(mLineConstants, (void *)&constants, sizeof(LineConstants), 0);
    const auto &[vbHandle, ibHandle] = mDebugMeshes[handle];
    m_device->BeginPass("Line Pass");
    m_device->BindSceneState(
        {.vb_handles = {vbHandle}, .cb_handles = {mLineConstants}, .ib_handle = ibHandle, .indexed = true});
    m_device->BindPipeline(m_line_pipeline);
    m_device->Draw(focus::Primitive::Lines, 0, mMeshManager->GetMesh(handle)->indices.size() / 2);
    m_device->EndPass();
}

} // namespace shared

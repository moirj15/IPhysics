#pragma once

#include "ShaderData.h"

#include <Common.h>
#include <Renderer/Camera.h>
#include <Utils/QuickCastBuffer.h>
#include <VoxelObjects/MeshManager.h>
#include <focus.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace shared
{

class Renderer
{
    focus::Device *m_device;
    // TODO: If this is still used change it into a struct for readability
    using BufferHandles = std::pair<focus::VertexBuffer, focus::IndexBuffer>;
    std::unordered_map<MeshHandle, BufferHandles> mMeshes;
    std::unordered_map<MeshHandle, BufferHandles> mDebugMeshes;
    MeshManager *mMeshManager;

    struct PhongConstants {
        glm::mat4 camera;
        glm::mat4 mvp;
        glm::mat4 normalMat;
        glm::vec4 lightPosition{4.0f, 1.0f, 2.0f, 1.0f};
    };

    struct PhongFragmentConstants {
        const glm::vec4 lightColor{1.0f, 1.0f, 1.0f, 1.0f};
        const glm::vec4 ambientLight{0.5f, 0.5f, 0.5f, 1.0f};
        const glm::vec4 ambientColor{1.0f, 1.0f, 0.7f, 1.0f};
        const glm::vec4 diffuseColor{0.0f, 1.0f, 0.7f, 1.0f};
        const glm::vec4 specularColor{1.0f, 1.0f, 1.0f, 1.0f};
        const glm::vec4 coeff{0.5f, 0.7f, 1.0f, 1.0f};
    } mPhongFragmentconstantsData;

    struct LineConstants {
        glm::mat4 mvp;
        glm::vec3 color;
    };

    focus::Shader mLineShader;
    focus::ConstantBuffer mLineConstants;

    focus::Pipeline m_line_pipeline;

    focus::Shader mPhongShader;
    focus::ConstantBuffer mPhongConstants;
    focus::ConstantBuffer mPhongFragmentConstants;

    focus::Pipeline m_phong_pipeline;

  public:
    explicit Renderer(focus::Device *device, MeshManager *meshManager) :
        m_device(device), mMeshManager(meshManager), mLineShader(m_device->CreateShaderFromSource("Line",
                                                         ReadFile("shaders/line.vert"), ReadFile("shaders/line.frag"))),
        m_line_pipeline(m_device->CreatePipeline({.shader = mLineShader})),
        mPhongShader(m_device->CreateShaderFromSource(
            "Phong", ReadFile("shaders/phongLight.vert"), ReadFile("shaders/phongLight.frag"))),
        m_phong_pipeline(m_device->CreatePipeline({.shader = mPhongShader}))
    {
        focus::ConstantBufferLayout phong_vertex_constants("constants");
        phong_vertex_constants.Add("camera", focus::VarType::Float4x4)
            .Add("mvp", focus::VarType::Float4x4)
            .Add("normalMat", focus::VarType::Float4x4)
            .Add("lightPosition", focus::VarType::Float4);
        mPhongConstants = m_device->CreateConstantBuffer(phong_vertex_constants, nullptr, sizeof(PhongConstants));

        focus::ConstantBufferLayout phong_fragment_constants("fragConstants");
        phong_fragment_constants.Add("lightColor", focus::VarType::Float4)
            .Add("ambientLight", focus::VarType::Float4)
            .Add("ambientColor", focus::VarType::Float4)
            .Add("diffuseColor", focus::VarType::Float4)
            .Add("coef", focus::VarType::Float4);

        mPhongFragmentConstants = m_device->CreateConstantBuffer(
            phong_fragment_constants, &mPhongFragmentconstantsData, sizeof(PhongFragmentConstants));

        focus::ConstantBufferLayout cb_layout("constants");
        cb_layout.Add("mvp", focus::VarType::Float4x4).Add("color", focus::VarType::Float3);
        mLineConstants = device->CreateConstantBuffer(cb_layout, nullptr, sizeof(LineConstants));

    }

    /**
     * @brief Load the mesh with the given handle from the MeshManager into vertex buffers
     *        for rendering. Will also load information into a debug vertex buffer for rendering
     *        voxels.
     * @param handle The handle of the mesh that will be loaded.
     */
    void LoadMesh(MeshHandle handle);
    void LoadDebugMesh(MeshHandle handle);

    void DrawMesh(MeshHandle handle, const Camera &camera, const glm::mat4 &model);
    void DrawDebugVoxels(MeshHandle handle, const Camera &camera, const glm::mat4 &model);
    void ClearScreen() { m_device->ClearBackBuffer({}); }
    void UpdateScreen(const focus::Window &window) { m_device->SwapBuffers(window); }
};

} // namespace shared

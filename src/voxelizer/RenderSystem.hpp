#pragma once

#include "System.hpp"
#include "Objects.h"
#include "glad.h"

#include <unordered_map>
#include <vector>

class Camera
{
    glm::vec3 m_position;
    glm::vec3 mTarget;
    glm::vec3 mUp;
    glm::vec3 mStrafe;
    glm::vec2 mAngles = {3.14f, 0.0f};

    mutable glm::mat4 mMatrix;

    mutable bool mRecalculate;

    const glm::vec3 X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 Y_AXIS = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);

public:
    explicit Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up) :
        m_position(position), mTarget(target), mUp(up), mStrafe(glm::normalize(glm::cross(mTarget, mUp))),
        mMatrix(1.0f), mRecalculate(true)
    {
    }

    void Move(const glm::vec3 &velocity);
    void Rotate(glm::vec2 angle);

    NODISCARD glm::mat4 CalculateMatrix() const;
    NODISCARD inline const glm::vec3 &GetPosition() const { return m_position; }
};

struct SDL_Window;

class RenderSystem : public System
{
    struct RenderData {
        u32 vertex_buffer_offset;
        u32 vertex_buffer_size;
        u32 index_buffer_offset;
        u32 index_buffer_size;
    };
    std::unordered_map<Handle<objs::Mesh>, RenderData> m_meshes;
    Camera m_camera;

    GLuint m_vao = GL_NONE;
    struct MainBuffer {
        GLuint vertex_buffer = GL_NONE;
        GLuint vb_size = 0;
        GLuint index_buffer = GL_NONE;
        GLuint ib_size = 0;
    } m_main_buffer;
    SDL_Window *m_window = nullptr;

    static constexpr f32 SPEED = 5.0f;

public:
    explicit RenderSystem(ResourceSystem *resource_system);
    void Step(f32 delta, const std::vector<Entity> &entities) override;
    void MeshLoaded(const Handle<objs::Mesh> &handle) override;
    SDL_Window *GetWindow() const { return m_window; }
};

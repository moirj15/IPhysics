#include "RenderSystem.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <SDL2/SDL.h>
#include "glad.h"

void Camera::Move(const glm::vec3 &velocity)
{
    mRecalculate = true;
    m_position += (mStrafe * velocity.x) + (mUp * velocity.y) + (mTarget * velocity.z);
}

void Camera::Rotate(glm::vec2 angle)
{
    mRecalculate = true;
    mAngles += glm::radians(angle * 0.005f);
    if (mAngles.y > glm::radians(89.0f)) {
        mAngles.y = glm::radians(89.0f);
    }
    if (mAngles.y < glm::radians(-89.0f)) {
        mAngles.y = glm::radians(-89.0f);
    }

    mTarget =
        glm::normalize(glm::vec3(cos(mAngles.y) * sin(mAngles.x), sin(mAngles.y), cos(mAngles.y) * cos(mAngles.x)));
    mStrafe = glm::cross(mTarget, mUp);
}

glm::mat4 Camera::CalculateMatrix() const
{
    if (mRecalculate) {
        mRecalculate = false;
        mMatrix = glm::lookAt(m_position, m_position + mTarget, mUp);
        return mMatrix;
    } else {
        return mMatrix;
    }
}

struct BufferLayout {
    glm::vec3 pos;
    glm::vec3 normal;
};

struct ConastantsLayout {
    glm::mat4 camera;
    glm::mat4 mvp;
    glm::mat4 normal_mat;
    glm::vec4 light_pos;
};

RenderSystem::RenderSystem() : m_camera({0, 0, 0}, {0, 0, -1}, {0, 1, 0})
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    constexpr u32 width = 1920;
    constexpr u32 height = 1080;

    m_window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

    assert(m_window != nullptr);

    // Don't resize window so it doesn't mess with tiling window managers
    SDL_SetWindowResizable(m_window, SDL_FALSE);

    auto *context = SDL_GL_CreateContext(m_window);
    assert(context);
    assert(gladLoadGL());

#ifdef _DEBUG
    // TODO: add debug flag
//    glEnable(GL_DEBUG_OUTPUT);
//    glDebugMessageCallback(DBCallBack, nullptr);
#endif
    glEnable(GL_PROGRAM_POINT_SIZE);

    printf("%s", glGetString(GL_VERSION));

    glGenVertexArrays(1, &m_vao);
}

void RenderSystem::Step(f32 delta)
{
    for (const auto &event : m_event_queue) {
        if (event == Event::MoveForward) {
            m_camera.Move(delta * SPEED * glm::vec3(0, 0, -1));
        } else if (event == Event::MoveBackwards) {
            m_camera.Move(delta * SPEED * glm::vec3(0, 0, 1));
        } else if (event == Event::MoveLeft) {
            m_camera.Move(delta * SPEED * glm::vec3(-1, 0, 0));
        } else if (event == Event::MoveRight) {
            m_camera.Move(delta * SPEED * glm::vec3(1, 0, 0));
        } else if (event == Event::MoveUp) {
            m_camera.Move(delta * SPEED * glm::vec3(0, 1, 0));
        } else if (event == Event::MoveDown) {
            m_camera.Move(delta * SPEED * glm::vec3(0, -1, 0));
        } else if (event == Event::MoveBoost) {
        }
    }
}

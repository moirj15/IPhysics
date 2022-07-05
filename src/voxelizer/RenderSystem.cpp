#include "RenderSystem.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>

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
}

void RenderSystem::Step()
{
    for (const auto &event : m_event_queue) {
        if (event == Event::MoveForward) {
            m_camera.Move(SPEED * glm::vec3(0, 0, -1));
        } else if (event == Event::MoveBackwards) {
            m_camera.Move(SPEED * glm::vec3(0, 0, 1));
        } else if (event == Event::MoveLeft) {
            m_camera.Move(SPEED * glm::vec3(-1, 0, 0));
        } else if (event == Event::MoveRight) {
            m_camera.Move(SPEED * glm::vec3(1, 0, 0));
        } else if (event == Event::MoveUp) {
            m_camera.Move(SPEED * glm::vec3(0, 1, 0));
        } else if (event == Event::MoveDown) {
            m_camera.Move(SPEED * glm::vec3(0, -1, 0));
        } else if (event == Event::MoveBoost) {
        }
    }
}

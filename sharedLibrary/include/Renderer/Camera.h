#pragma once

#include "../Common.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

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

#ifndef CAMERA_H
#define CAMERA_H

#include "../common.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
  glm::vec3 m_position;
  glm::vec3 m_target;
  glm::vec3 m_up;

  glm::mat4 m_matrix;
  glm::mat4 m_rotation;
  glm::mat4 m_translation;

  bool m_updateMatrix;

public:
  /// Default Constructor
  Camera() :
      m_position{0.0f, 0.0f, 0.0f}, m_target{0.0f, 0.0f, -1.0f}, m_up{0.0f, 1.0f, 0.0f},
      m_matrix{1.0f}, m_rotation{1.0f}, m_translation{1.0f}, m_updateMatrix{true} {}

  /**
   * @brief Camera: Constructor
   * @param position: The position of the camera.
   * @param target: The camera target.
   * @param up: The up vector.
   */
  Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up) :
      m_position{position}, m_target{target}, m_up{up}, m_matrix{1.0f}, m_rotation{1.0f},
      m_translation{1.0f}, m_updateMatrix{true} {}

  /**
   * @brief CalculateMatrix: Calculates a new camera matrix if needed, or returns the previous
   * matrix.
   * @return: The camera matrix.
   */
  glm::mat4 CalculateMatrix();

  /**
   * @brief Rotate: Rotates the Camera about an axis.
   * @param angle: The angle of rotation in degrees.
   * @param axis: The axis of rotation.
   */
  void Rotate(const f32 angle, const glm::vec3 axis);

  /**
   * @brief Rotate: Rotates the camera using the given quaternion.
   * @param rotation: The quaternion.
   */
  void Rotate(const glm::quat &rotation);

  /**
   * @brief Rotate: Rotates the camera using the given rotation matrix.
   * @param rotation: The rotation matrix.
   */
  void Rotate(const glm::mat4 &rotation);

  /**
   * @brief Translate: Translates the camera's position using the given position vector.
   * @param newPosition: The new position vector.
   */
  void Translate(const glm::vec3 &newPosition);

  /**
   * @brief Translate: Translates the camera's position using the given matrix.
   * @param translation: The matrix used to translate the camera.
   */
  void Translate(const glm::mat4 &translation);

  /**
   * @brief Reset: Sets the camera to the values defined in the default constructor.
   */
  void Reset();
};

#endif // CAMERA_H

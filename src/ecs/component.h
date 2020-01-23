#pragma once

#include "../common.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

struct Mesh;

namespace ecs
{

constexpr u64 RENDERABLE_BIT = 1 << 32;
constexpr u64 TRANSFORM_BIT = 1 << 33;
constexpr u64 PHYSICS_BIT = 1 << 34;

struct Renderable
{
  u32 mMeshHandle;
  u32 mTextureHandle;
  Mesh *mMesh;
};

struct Transform
{
  glm::quat mRotation;
  glm::vec3 mPosition;
};

struct Physics
{
  bool mCollision;
  // TODO: rest of the members
};
} // namespace ecs

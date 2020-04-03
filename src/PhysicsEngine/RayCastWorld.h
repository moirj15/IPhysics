#pragma once
#include "../common.h"
#include "Ray.h"
#include "btBulletCollisionCommon.h"

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btSimpleDynamicsWorld.h>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

struct Box
{
  btVector3 mOrigin;
  btVector3 mExtents;

  Box(const glm::vec3 &origin, const glm::vec3 &extents) :
      mOrigin(origin.x, origin.y, origin.z), mExtents(extents.x, extents.y, extents.z)
  {
  }
};

class RayCastWorld
{
  std::unique_ptr<btDefaultCollisionConfiguration> mCollisionConfig;
  std::unique_ptr<btCollisionDispatcher> mCollisionDispatcher;
  std::unique_ptr<btBroadphaseInterface> mOverlappingPairCache;
  std::unique_ptr<btSequentialImpulseConstraintSolver> mSolver;
  std::unique_ptr<btCollisionWorld> mCollisionWorld;
  std::vector<std::unique_ptr<btRigidBody>> mRigidBodies;

public:
  RayCastWorld() :
      mCollisionConfig(new btDefaultCollisionConfiguration),
      mCollisionDispatcher(new btCollisionDispatcher(mCollisionConfig.get())),
      mOverlappingPairCache(new btDbvtBroadphase()),
      mSolver(new btSequentialImpulseConstraintSolver()),
      mCollisionWorld(new btDiscreteDynamicsWorld(
          mCollisionDispatcher.get(), mOverlappingPairCache.get(), mSolver.get(),
          mCollisionConfig.get()))
  {
  }

  void AddBoxes(std::vector<Box> &boxes);
  std::vector<glm::vec3> CastRay(const Ray &ray);
};

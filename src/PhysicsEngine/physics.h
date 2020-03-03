#pragma once

#include "../common.h"
#include "../utils/VoxelMeshManager.h"
#include "Settings.h"
#include "btBulletCollisionCommon.h"

#include <BulletDynamics/ConstraintSolver/btConeTwistConstraint.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btSimpleDynamicsWorld.h>

/// Vehicle simulation, with wheel contact simulated by raycasts
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"

#include <memory>
// #include <reactphysics3d.h>
#include <vector>

class Object;
class Voxel;

namespace Physics
{

class PhysicsEngine
{
  std::vector<VMeshHandle> mObjectHandles;

  std::unordered_map<VMeshHandle, std::unique_ptr<btCollisionShape>> mObjects;

  std::unique_ptr<btDefaultCollisionConfiguration> mCollisionConfig;
  std::unique_ptr<btCollisionDispatcher> mCollisionDispatcher;
  std::unique_ptr<btBroadphaseInterface> mOverlappingPairCache;
  std::unique_ptr<btSequentialImpulseConstraintSolver> mSolver;
  std::unique_ptr<btDiscreteDynamicsWorld> mDynamicsWorld;

  EngineSettings mSettings;

public:
  PhysicsEngine() :
      mCollisionConfig(new btDefaultCollisionConfiguration),
      mCollisionDispatcher(new btCollisionDispatcher(mCollisionConfig.get())),
      mOverlappingPairCache(new btDbvtBroadphase()),
      mSolver(new btSequentialImpulseConstraintSolver()),
      mDynamicsWorld(new btDiscreteDynamicsWorld(
          mCollisionDispatcher.get(), mOverlappingPairCache.get(), mSolver.get(),
          mCollisionConfig.get()))
  {
    mDynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));
  }

  void Update(f32 t);

  void SubmitObject(const VMeshHandle handle);
  void RemoveObject(const VMeshHandle handle);

  void CastRayWithForce(const glm::vec3 &origin, const glm::vec3 &direction, f32 force);

  inline void SetEngineSettings(EngineSettings engineSettings)
  {
    mSettings = engineSettings;
  }

private:
  void CollectVoxelsWithBadPositions();
  void CorrectVoxelPositions();
};

} // namespace Physics
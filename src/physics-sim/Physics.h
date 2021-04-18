#pragma once

#include <Common.h>
#include <Renderer/DebugDrawer.h>
#include <VoxelObjects/Objects.h>
#include "Settings.h"
#include "btBulletCollisionCommon.h"

#include <BulletDynamics/ConstraintSolver/btConeTwistConstraint.h>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/ConstraintSolver/btUniversalConstraint.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btSimpleDynamicsWorld.h>

/// Vehicle simulation, with wheel contact simulated by raycasts
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"

#include <memory>
// #include <reactphysics3d.h>
#include <vector>
#include <VoxelObjects/MeshManager.h>

class Object;

namespace Physics
{

class PhysicsEngine
{
  MeshManager mPhysicsMeshManager;


  std::unordered_map<MeshHandle, ObjectSettings> mObjectSettings;
  std::vector<MeshHandle> mObjectHandles;

  // Shared position of the mesh and voxel-mesh
  std::unordered_map<MeshHandle, glm::vec3> mObjectPositions;
  std::unordered_map<MeshHandle, std::unique_ptr<btRigidBody>> mObjects;
  std::unordered_map<MeshHandle, std::vector<std::unique_ptr<btRigidBody>>> mVoxels;
  Renderer::DebugDrawer *mDebugDrawer;

  // TODO: Do i really need two Dynamics worlds?
  // may be possible to do everything with only one world and it may be faster
  struct PhysicsWorld
  {
    std::unique_ptr<btDefaultCollisionConfiguration> mCollisionConfig;
    std::unique_ptr<btCollisionDispatcher> mCollisionDispatcher;
    std::unique_ptr<btBroadphaseInterface> mOverlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> mSolver;
    std::unique_ptr<btDiscreteDynamicsWorld> mDynamicsWorld;

    PhysicsWorld() :
        mCollisionConfig(new btDefaultCollisionConfiguration),
        mCollisionDispatcher(new btCollisionDispatcher(mCollisionConfig.get())),
        mOverlappingPairCache(new btDbvtBroadphase()),
        mSolver(new btSequentialImpulseConstraintSolver()),
        mDynamicsWorld(new btDiscreteDynamicsWorld(
            mCollisionDispatcher.get(), mOverlappingPairCache.get(), mSolver.get(),
            mCollisionConfig.get()))
    {
    }
    ~PhysicsWorld() = default;

    void Reset()
    {
      mDynamicsWorld.release();
      mSolver.reset(new btSequentialImpulseConstraintSolver());
      mCollisionConfig.reset(new btDefaultCollisionConfiguration);
      mCollisionDispatcher.reset(new btCollisionDispatcher(mCollisionConfig.get()));
      mOverlappingPairCache.reset(new btDbvtBroadphase());
      mDynamicsWorld.reset(new btDiscreteDynamicsWorld(
          mCollisionDispatcher.get(), mOverlappingPairCache.get(), mSolver.get(),
          mCollisionConfig.get()));
    }

  } mObjectWorld, mVoxelWorld;

  EngineSettings mSettings;

public:
  PhysicsEngine(Renderer::DebugDrawer *db) : mDebugDrawer(db)
  {
    Init();
  }

  void Reset();

  void Update(f32 t);

  inline void SetInitialWorldState(const MeshManager& meshManager, const std::unordered_map<MeshHandle, ObjectSettings> &settings)
  {
    mPhysicsMeshManager = meshManager;
    mObjectSettings = settings;
    // TODO: doing this the lazy way, clean up later
    
  }

  void SubmitObject(MeshHandle handle);
  void RemoveObject(MeshHandle handle);

  void CastRayWithForce(
      const glm::vec3 &rayStartNDC, const glm::vec3 &rayEndNDC, const glm::mat4 &NDCToWorldSpace,
      f32 force);

  inline void SetEngineSettings(EngineSettings engineSettings)
  {
    mSettings = engineSettings;
  }

  inline const std::unordered_map<MeshHandle, glm::vec3>& GetPositions() {
    return mObjectPositions;
  }

private:
  void Init();
  void AddObject(MeshHandle handle, btCompoundShape *collisionShape);
  btCompoundShape *AddVoxels(MeshHandle handle);
  void AdjustVoxelSizeFromImpulse(
      objs::Voxel *voxel, f32 impulse, u32 modifiedDimm, u32 unchangedDimm0, u32 unchangedDimm1);
};

} // namespace Physics
#include "physics.h"

#include "../voxelObjects/VoxelMesh.h"

namespace Physics
{

// void TickCallback(btDynamicsWorld *dynamicsWorld, btScalar timeStep)
// {
//   s32 manifoldCount = dynamicsWorld->getDispatcher()->getNumManifolds();
// }

void PhysicsEngine::Update(f32 t)
{
  mObjectWorld.mDynamicsWorld->stepSimulation(t, 10);
  for (const auto handle : mObjectHandles)
  {
    // Update our object position in its settings, so we can render it in the right spot
    auto *objectSettings = VoxelMeshManager::Get().GetSettings(handle);
    btRigidBody *rb = (btRigidBody *)mObjects[handle].get();
    const auto &transform = rb->getWorldTransform();
    const auto &position = transform.getOrigin();
    objectSettings->mPosition = glm::vec3(position.x(), position.y(), position.z());
  }
  // Find all the colliding bodies in the voxel world
  u32 manifoldCount = mVoxelWorld.mCollisionDispatcher->getNumManifolds();
  for (u32 i = 0; i < manifoldCount; i++)
  {
    auto *contactManifold = mVoxelWorld.mCollisionDispatcher->getManifoldByIndexInternal(i);
    const auto *objA = contactManifold->getBody0();
    const auto *objB = contactManifold->getBody1();
    auto positionA = objA->getWorldTransform().getOrigin();
    auto positionB = objB->getWorldTransform().getOrigin();

    auto voxelA = (VoxObj::Voxel *)objA->getUserPointer();
    auto voxelB = (VoxObj::Voxel *)objB->getUserPointer();

    voxelA->mPosition = glm::vec3(positionA.x(), positionA.y(), positionA.z());
    voxelB->mPosition = glm::vec3(positionB.x(), positionB.y(), positionB.z());
  }
  // mObjectWorld.mDynamicsWorld->debugDrawWorld();
  mVoxelWorld.mDynamicsWorld->debugDrawWorld();
}

void PhysicsEngine::SubmitObject(const VMeshHandle handle)
{
  mObjectHandles.emplace_back(handle);
  AddObject(handle);
  AddVoxels(handle);
}

void PhysicsEngine::RemoveObject(const VMeshHandle handle)
{
  IMPLEMENTME();
}

void PhysicsEngine::CastRayWithForce(const glm::vec3 &origin, const glm::vec3 &direction, f32 force)
{
  btVector3 rayOrigin(origin.x, origin.y, origin.z);
  btVector3 rayDirection(direction.x, direction.y, direction.z);
  auto rayEnd = rayOrigin + rayDirection * 1000.0f;
  btCollisionWorld::ClosestRayResultCallback rayCallback(rayOrigin, rayEnd);
  mObjectWorld.mDynamicsWorld->rayTest(rayOrigin, rayEnd, rayCallback);
  if (rayCallback.hasHit())
  {
    auto *rigidBody = (btRigidBody *)rayCallback.m_collisionObject;
    rigidBody->activate(true);
    // rigidBody->applyCentralImpulse(rayDirection * force);
    rigidBody->applyCentralImpulse(btVector3(1.0, 0.0, 0.0) * force);
  }
}

void PhysicsEngine::AddObject(const VMeshHandle handle)
{
  auto *vMesh = VoxelMeshManager::Get().GetMesh(handle);
  auto *objectSettings = VoxelMeshManager::Get().GetSettings(handle);
  auto extentsObjectSpace = vMesh->GetExtentsObjectSpace();

  btCollisionShape *objectCollisionShape = new btBoxShape(btVector3(
      extentsObjectSpace.x / 2.0f, extentsObjectSpace.y / 2.0f, extentsObjectSpace.z / 2.0f));

  f32 mass = 1.0f;
  btVector3 localInteria(0.0f, 0.0f, 0.0f);
  objectCollisionShape->calculateLocalInertia(1.0f, localInteria);

  auto *motionState = new btDefaultMotionState(btTransform(
      btQuaternion(),
      btVector3(
          objectSettings->mPosition.x, objectSettings->mPosition.y, objectSettings->mPosition.z)));

  btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(
      1.0f, motionState, objectCollisionShape, btVector3(0.0f, 0.0f, 0.0f));

  auto rigidBody = new btRigidBody(rigidBodyInfo);
  rigidBody->setUserIndex(handle);

  mObjectWorld.mDynamicsWorld->addRigidBody(rigidBody);
  mObjects.emplace(handle, rigidBody);
}

void PhysicsEngine::AddVoxels(const VMeshHandle handle)
{
  auto *vMesh = VoxelMeshManager::Get().GetMesh(handle);
  auto *objectSettings = VoxelMeshManager::Get().GetSettings(handle);
  mVoxels.emplace(handle, std::vector<std::unique_ptr<btRigidBody>>());
  for (auto &[key, voxel] : vMesh->GetVoxels())
  {
    // Update the initial voxel position
    voxel.mPosition += objectSettings->mPosition;

    // Do all the bullet object creation stuff
    btCollisionShape *objectCollisionShape = new btBoxShape(btVector3(
        voxel.mDimensions.x / 2.0f, voxel.mDimensions.y / 2.0f, voxel.mDimensions.z / 2.0f));

    f32 mass = 1.0f;
    btVector3 localInteria(0.0f, 0.0f, 0.0f);
    objectCollisionShape->calculateLocalInertia(1.0f, localInteria);

    auto *motionState = new btDefaultMotionState(btTransform(
        btQuaternion(), btVector3(voxel.mPosition.x, voxel.mPosition.y, voxel.mPosition.z)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(
        1.0f, motionState, objectCollisionShape, btVector3(0.0f, 0.0f, 0.0f));

    auto rigidBody = new btRigidBody(rigidBodyInfo);
    rigidBody->setUserIndex(handle);

    // set the user index so we can get this voxel at a later time
    rigidBody->setUserIndex(handle);
    rigidBody->setUserIndex2(mVoxels[handle].size());
    rigidBody->setUserPointer((void *)&voxel);

    // Add the object to the voxel world so it will only interact with other voxels
    mVoxelWorld.mDynamicsWorld->addRigidBody(rigidBody);
    mVoxels[handle].emplace_back(rigidBody);
  }
}

void PhysicsEngine::CollectVoxelsWithBadPositions()
{
}

void PhysicsEngine::CorrectVoxelPositions()
{
}

} // namespace Physics

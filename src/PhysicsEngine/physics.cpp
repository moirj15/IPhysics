#include "physics.h"

#include "../voxelObjects/VoxelMesh.h"

namespace Physics
{

void PhysicsEngine::Update(f32 t)
{
}

void PhysicsEngine::SubmitObject(const VMeshHandle handle)
{
  mObjectHandles.emplace_back(handle);
  auto *vMesh = VoxelMeshManager::Get().GetMesh(handle);
  auto *objectSettings = VoxelMeshManager::Get().GetSettings(handle);
  auto extentsObjectSpace = vMesh->GetExtentsObjectSpace();

  btCollisionShape *objectCollisionShape = new btBoxShape(btVector3(
      extentsObjectSpace.x / 2.0f, extentsObjectSpace.y / 2.0f, extentsObjectSpace.z / 2.0f));

  f32 mass = 1.0f;
  btVector3 localInteria(0.0f, 0.0f, 0.0f);
  objectCollisionShape->calculateLocalInertia(1.0f, localInteria);

  btTransform initialTransform;
  initialTransform.setIdentity();
  initialTransform.setOrigin(btVector3(
      objectSettings->mPosition.x, objectSettings->mPosition.y, objectSettings->mPosition.z));

  auto *motionState = new btDefaultMotionState(initialTransform);

  btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(
      1.0f, motionState, objectCollisionShape, btVector3(0.0f, 0.0f, 0.0f));

  auto rigidBody = new btRigidBody(rigidBodyInfo);
  rigidBody->setUserIndex(handle);

  mDynamicsWorld->addRigidBody(rigidBody);
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
  btCollisionWorld::ClosestRayResultCallback rayCallback(rayOrigin, rayDirection);
  mDynamicsWorld->rayTest(rayOrigin, rayDirection, rayCallback);
  if (rayCallback.hasHit())
  {
    auto *rigidBody = (btRigidBody *)rayCallback.m_collisionObject;
    rigidBody->applyCentralForce(rayDirection);
    printf("OUCH\n");
  }
}

void PhysicsEngine::CollectVoxelsWithBadPositions()
{
}

void PhysicsEngine::CorrectVoxelPositions()
{
}

} // namespace Physics

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
  mVoxelWorld.mDynamicsWorld->stepSimulation(t, 10);
  for (const auto handle : mObjectHandles)
  {
    // Update our object position in its settings, so we can render it in the right spot
    auto *objectSettings = VoxelMeshManager::Get().GetSettings(handle);
    auto *rb = (btRigidBody *)mObjects[handle].get();
    const auto &transform = rb->getWorldTransform();
    const auto &position = transform.getOrigin();
    objectSettings->mPosition = glm::vec3(position.x(), position.y(), position.z());
  }
  for (const auto &[key, voxelRBs] : mVoxels)
  {
    auto *objectSettings = VoxelMeshManager::Get().GetSettings(key);
    for (auto &voxelRB : voxelRBs)
    {
      const auto &origin = voxelRB->getWorldTransform().getOrigin();
      glm::vec3 voxelCurrPosition(origin.x(), origin.y(), origin.z());
      auto *voxel = (VoxObj::Voxel *)voxelRB->getUserPointer();
      voxel->mPosition = voxelCurrPosition;
      voxel->mRelativePositionDelta =
          (voxel->mPosition - objectSettings->mPosition) - voxel->mPositionRelativeToCenter;
      voxel->mPositionRelativeToCenter = voxel->mPosition - objectSettings->mPosition;
    }
  }
  mObjectWorld.mDynamicsWorld->debugDrawWorld();
  // mVoxelWorld.mDynamicsWorld->debugDrawWorld();
}

void PhysicsEngine::SubmitObject(const VMeshHandle handle)
{
  mObjectHandles.emplace_back(handle);
  auto *collisionShape = AddObject(handle);
  AddVoxels(handle, collisionShape);
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

    auto handle = (VMeshHandle)rigidBody->getUserIndex();
    for (auto &voxelRB : mVoxels[handle])
    {
      voxelRB->applyCentralImpulse(btVector3(1.0, 0.0, 0.0) * force);
      voxelRB->activate(true);
    }
  }
}

btCompoundShape *PhysicsEngine::AddObject(const VMeshHandle handle)
{
  auto *vMesh = VoxelMeshManager::Get().GetMesh(handle);
  auto *objectSettings = VoxelMeshManager::Get().GetSettings(handle);
  auto extentsObjectSpace = vMesh->GetExtentsObjectSpace();

  // auto *collisionShape = new btCompoundShape(btVector3(
  //     extentsObjectSpace.x / 2.0f, extentsObjectSpace.y / 2.0f, extentsObjectSpace.z / 2.0f));
  auto *collisionShape = new btCompoundShape(true, vMesh->mVoxels.size());

  f32 mass = 1.0f;
  btVector3 localInteria(0.0f, 0.0f, 0.0f);
  collisionShape->calculateLocalInertia(1.0f, localInteria);

  auto *motionState = new btDefaultMotionState(btTransform(
      btQuaternion(),
      btVector3(
          objectSettings->mPosition.x, objectSettings->mPosition.y, objectSettings->mPosition.z)));

  btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(
      1.0f, motionState, collisionShape, btVector3(0.0f, 0.0f, 0.0f));

  auto rigidBody = new btRigidBody(rigidBodyInfo);
  rigidBody->setUserIndex(handle);

  mObjectWorld.mDynamicsWorld->addRigidBody(rigidBody);
  mObjects.emplace(handle, rigidBody);
  return collisionShape;
}

void PhysicsEngine::AddVoxels(const VMeshHandle handle, btCompoundShape *collisionShape)
{
  auto *vMesh = VoxelMeshManager::Get().GetMesh(handle);
  auto *objectSettings = VoxelMeshManager::Get().GetSettings(handle);
  mVoxels.emplace(handle, std::vector<std::unique_ptr<btRigidBody>>());
  std::unordered_map<glm::uvec3, btRigidBody *> neighbors;
  for (auto &[key, voxel] : vMesh->mVoxels)
  {

    // Do all the bullet object creation stuff
    btCollisionShape *voxelCollisionShape = new btBoxShape(btVector3(
        voxel.mDimensions.x / 2.0f, voxel.mDimensions.y / 2.0f, voxel.mDimensions.z / 2.0f));
    // Add the voxel to the btCompundShape
    collisionShape->addChildShape(
        btTransform(
            btQuaternion(), btVector3(voxel.mPosition.x, voxel.mPosition.y, voxel.mPosition.z)),
        voxelCollisionShape);
    // Update the initial voxel position
    voxel.mPosition += objectSettings->mPosition;
    voxel.mPositionRelativeToCenter = voxel.mPosition - objectSettings->mPosition;

    f32 mass = 1.0f;
    btVector3 localInteria(0.0f, 0.0f, 0.0f);
    voxelCollisionShape->calculateLocalInertia(1.0f, localInteria);

    auto *motionState = new btDefaultMotionState(btTransform(
        btQuaternion(), btVector3(voxel.mPosition.x, voxel.mPosition.y, voxel.mPosition.z)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(
        1.0f, motionState, voxelCollisionShape, btVector3(0.0f, 0.0f, 0.0f));

    auto rigidBody = new btRigidBody(rigidBodyInfo);
    rigidBody->setUserIndex(handle);

    // set the user index so we can get this voxel at a later time
    rigidBody->setUserIndex(handle);
    rigidBody->setUserIndex2(mVoxels[handle].size());
    rigidBody->setUserPointer((void *)&vMesh->mVoxels[key]);

    // Add the object to the voxel world so it will only interact with other voxels
    mVoxelWorld.mDynamicsWorld->addRigidBody(rigidBody);

    mVoxels[handle].emplace_back(rigidBody);
    neighbors.emplace(key, rigidBody);
  }
  for (auto &[key, voxel] : vMesh->mVoxels)
  {
    auto size = voxel.mDimensions;
    // TODO: don't create double constraints
    for (const auto &n : voxel.mNeighbors)
    {
      glm::vec3 ap(glm::abs(glm::vec3(n) - glm::vec3(key)));
      glm::vec3 bp(-ap);
      auto *voxelRB = neighbors[key];
      auto *neighbor = neighbors[n];
      auto a = btTransform::getIdentity();
      // a.setOrigin(btVector3(ap.x, ap.y, ap.z));
      auto b = btTransform::getIdentity();
      // b.setOrigin(btVector3(bp.x, bp.y, bp.z));
      auto *constraint = new btGeneric6DofSpringConstraint(*voxelRB, *neighbor, a, b, true);
      // TODO: fix limits so they're relative to the voxel's initials size
      constraint->setLinearLowerLimit(
          btVector3(-size.x, -size.y, -size.z) + btVector3(ap.x, ap.y, ap.z));
      constraint->setLinearUpperLimit(
          btVector3(size.x, size.y, size.z) - btVector3(ap.x, ap.y, ap.z));
      // constraint->setLimit(2, 0.0, 1.0);
      // mVoxelWorld.mDynamicsWorld->addConstraint(constraint, false);
      mObjectWorld.mDynamicsWorld->addConstraint(constraint, false);
    }
  }
}

void PhysicsEngine::CollectVoxelsWithBadPositions()
{
}

void PhysicsEngine::CorrectVoxelPositions()
{
}

} // namespace Physics

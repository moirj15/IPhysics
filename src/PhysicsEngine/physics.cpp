#include "physics.h"

#include "../voxelObjects/VoxelMesh.h"

namespace Physics
{

void PhysicsEngine::Update(f32 t)
{
  mObjectWorld.mDynamicsWorld->stepSimulation(t, 10);
  mVoxelWorld.mDynamicsWorld->stepSimulation(t, 10);
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
  for (const auto handle : mObjectHandles)
  {
    // Update our object position in its settings, so we can render it in the right spot
    auto *objectSettings = VoxelMeshManager::Get().GetSettings(handle);
    auto *rb = (btRigidBody *)mObjects[handle].get();
    const auto &transform = rb->getWorldTransform();
    const auto &position = transform.getOrigin();
    objectSettings->mPosition = glm::vec3(position.x(), position.y(), position.z());
    auto *collisionShape = (btCompoundShape *)rb->getCollisionShape();
    for (s32 i = 0; i < collisionShape->getNumChildShapes(); i++)
    {
      auto *childCollisionShape = (btBoxShape *)collisionShape->getChildShape(i);
      auto *voxel = (VoxObj::Voxel *)childCollisionShape->getUserPointer();
      auto tmp = collisionShape->getChildTransform(i);
      btTransform newTransform(
          btQuaternion(),
          btVector3(
              voxel->mPositionRelativeToCenter.x, voxel->mPositionRelativeToCenter.y,
              voxel->mPositionRelativeToCenter.z));
      collisionShape->updateChildTransform(i, newTransform);
    }
  }
  mObjectWorld.mDynamicsWorld->debugDrawWorld();
  // mVoxelWorld.mDynamicsWorld->debugDrawWorld();
}

void PhysicsEngine::SubmitObject(const VMeshHandle handle)
{
  mObjectHandles.emplace_back(handle);
  auto *collisionShape = AddVoxels(handle);
  AddObject(handle, collisionShape);
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

void PhysicsEngine::AddObject(const VMeshHandle handle, btCompoundShape *collisionShape)
{
  auto *vMesh = VoxelMeshManager::Get().GetMesh(handle);
  auto *objectSettings = VoxelMeshManager::Get().GetSettings(handle);

  btTransform transform = btTransform::getIdentity();
  transform.setOrigin(btVector3(
      objectSettings->mPosition.x, objectSettings->mPosition.y, objectSettings->mPosition.z));

  auto *motionState = new btDefaultMotionState(transform);

  btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(1.0f, motionState, collisionShape);

  auto rigidBody = new btRigidBody(rigidBodyInfo);
  rigidBody->setUserIndex(handle);

  mObjectWorld.mDynamicsWorld->addRigidBody(rigidBody);
  mObjects.emplace(handle, rigidBody);
}

btCompoundShape *PhysicsEngine::AddVoxels(const VMeshHandle handle)
{
  auto *vMesh = VoxelMeshManager::Get().GetMesh(handle);
  auto *objectSettings = VoxelMeshManager::Get().GetSettings(handle);
  mVoxels.emplace(handle, std::vector<std::unique_ptr<btRigidBody>>());
  std::unordered_map<glm::uvec3, btRigidBody *> neighbors;
  auto *collisionShape = new btCompoundShape(true, vMesh->mVoxels.size());
  for (auto &[key, voxel] : vMesh->mVoxels)
  {

    // Do all the bullet object creation stuff
    btCollisionShape *voxelCollisionShape = new btBoxShape(btVector3(
        voxel.mDimensions.x / 2.0f, voxel.mDimensions.y / 2.0f, voxel.mDimensions.z / 2.0f));
    // Add the voxel to the btCompundShape
    voxelCollisionShape->setUserPointer((void *)&vMesh->mVoxels[key]);
    // Update the initial voxel position
    voxel.mPosition += objectSettings->mPosition;

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

    //
    collisionShape->addChildShape(rigidBody->getWorldTransform(), voxelCollisionShape);

    // Add the object to the voxel world so it will only interact with other voxels
    mVoxelWorld.mDynamicsWorld->addRigidBody(rigidBody);

    mVoxels[handle].emplace_back(rigidBody);
    neighbors.emplace(key, rigidBody);
  }
  std::vector<f32> masses(vMesh->mVoxels.size(), 1.0f);
  btTransform principal;
  btVector3 inertia;
  collisionShape->calculatePrincipalAxisTransform(masses.data(), principal, inertia);
  for (s32 i = 0; i < collisionShape->getNumChildShapes(); i++)
  {
    collisionShape->getChildTransform(i) *= principal.inverse();
    auto *child = collisionShape->getChildShape(i);
    auto *voxel = (VoxObj::Voxel *)child->getUserPointer();
    voxel->mPositionRelativeToCenter = voxel->mPosition - objectSettings->mPosition;
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
      constraint->setLinearLowerLimit(
          btVector3(-size.x, -size.y, -size.z) + btVector3(ap.x, ap.y, ap.z));
      constraint->setLinearUpperLimit(
          btVector3(size.x, size.y, size.z) - btVector3(ap.x, ap.y, ap.z));
      // constraint->setLimit(2, 0.0, 1.0);
      mVoxelWorld.mDynamicsWorld->addConstraint(constraint, false);
    }
  }
  return collisionShape;
}

} // namespace Physics

#include "Physics.h"

#include <Utils/MathCasts.h>
#include <VoxelObjects/VoxelMesh.h>

#include <glm/gtx/component_wise.hpp>
#include <unordered_set>

namespace Physics
{

void PhysicsEngine::Reset()
{
  mObjectWorld.Reset();
  mVoxelWorld.Reset();
  mVoxels.clear();
  mObjects.clear();
  mObjectHandles.clear();
  Init();
}

void PhysicsEngine::Update(f32 t)
{
  mObjectWorld.mDynamicsWorld->stepSimulation(t, 10);
  mVoxelWorld.mDynamicsWorld->stepSimulation(t, 10);
  // TODO: add a bool to make this optional
  auto &voxelDispatcher = mVoxelWorld.mCollisionDispatcher;
  if (mSettings.mEnableExtension)
  {
    s32 numManifolds = voxelDispatcher->getNumManifolds();
    for (s32 i = 0; i < numManifolds; i++)
    {
      auto *contactManifold = voxelDispatcher->getManifoldByIndexInternal(i);
      s32 numContacts = contactManifold->getNumContacts();
      for (s32 j = 0; j < numContacts; j++)
      {
        auto contactPoint = contactManifold->getContactPoint(j);
        f32 impulse = contactPoint.getAppliedImpulse();
        auto *voxelRB = contactManifold->getBody1();
        auto *voxel = (VoxObj::Voxel *)voxelRB->getUserPointer();
        // TODO: store the mass of the voxel, since we can't get this from bullet
        f32 density = glm::compMul(voxel->mDimensions) * 1.0f;
        const f32 impulseThreshold = density; // * 10.0f;
        if (impulse > impulseThreshold)
        {
          // Use the contactPoint's B normal to determine which voxel face to adjust
          auto contactNormal = ToGLM(contactPoint.m_normalWorldOnB);
          //         contactNormal *= impulse;
          //         printf("impulse * t %f\n", impulse * t * 0.01f);
          for (u32 v = 0; v < voxel->mBezierCurves.size(); v++)
          {
            auto &bezierCurve = voxel->mBezierCurves[v];
            // TODO: compare these two methods of modifying the bezier curves
            //           for (auto &cp : bezierCurve.mControlPoints)
            //           {
            //             cp += impulseDirection;
            //           }
            auto &firstCP = bezierCurve.mControlPoints[0];
            auto &secondCP = bezierCurve.mControlPoints[bezierCurve.mControlPoints.size() - 1];
            auto firstCPNormal = glm::normalize(firstCP - voxel->mPosition);
            auto secondCPNormal = glm::normalize(secondCP - voxel->mPosition);
            // check if the normalized control point and the contact normal point in the same
            // direction
            if (glm::dot(firstCPNormal, contactNormal) > 0.0f)
            {
              firstCP += (-contactNormal * impulse) * t; // * t;
            }
            if (glm::dot(secondCPNormal, contactNormal) > 0.0f)
            {
              secondCP += (-contactNormal * impulse) * t; // * t;
            }

            //           bezierCurve.mControlPoints[0] += contactNormal;
            //           bezierCurve.mControlPoints[bezierCurve.mControlPoints.size() - 1] +=
            //           contactNormal;
          }
        }
      }
    }
  }
  for (const auto &[key, voxelRBs] : mVoxels)
  {
    auto *objectSettings = VoxelMeshManager::Get().GetSettings(key);
    for (auto &voxelRB : voxelRBs)
    {

      const auto &origin = voxelRB->getWorldTransform().getOrigin();
      glm::vec3 voxelCurrPosition(origin.x(), origin.y(), origin.z());
      auto *voxel = (VoxObj::Voxel *)voxelRB->getUserPointer();
      auto orignalPosition = voxel->mPosition;
      voxel->mPosition = voxelCurrPosition;
      auto toNewPosition = voxel->mPosition - orignalPosition;
      // TODO: may have to move this to before the dimensions of the voxel are changed.
      // voxel->UpdateBezierCurves(toNewPosition);
      voxel->mRelativePositionDelta +=
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

void PhysicsEngine::CastRayWithForce(
    const glm::vec3 &rayStartNDC, const glm::vec3 &rayEndNDC, const glm::mat4 &NDCToWorldSpace,
    f32 force)
{
  auto rayStartWorld = NDCToWorldSpace * glm::vec4(rayStartNDC, 1.0f);
  rayStartWorld /= rayStartWorld.w;

  auto rayEndWorld = NDCToWorldSpace * glm::vec4(rayEndNDC, 1.0f);
  rayEndWorld /= rayEndWorld.w;

  auto rayDirWorld = glm::normalize(rayEndWorld - rayStartWorld);
  btVector3 rayOrigin(rayStartWorld.x, rayStartWorld.y, rayStartWorld.z);
  btVector3 rayDirection(rayEndWorld.x, rayEndWorld.y, rayEndWorld.z);
  auto rayEnd = rayOrigin + rayDirection * 1000.0f;
  btCollisionWorld::ClosestRayResultCallback rayCallback(rayOrigin, rayEnd);
  mObjectWorld.mDynamicsWorld->rayTest(rayOrigin, rayEnd, rayCallback);
  if (rayCallback.hasHit())
  {
    auto *rigidBody = (btRigidBody *)rayCallback.m_collisionObject;
    rigidBody->activate(true);
    rigidBody->applyCentralImpulse(rayDirection.normalize() * force);
    //     rigidBody->applyCentralImpulse(btVector3(1.0, 0.0, 0.0) * force);

    auto handle = (VMeshHandle)rigidBody->getUserIndex();
    for (auto &voxelRB : mVoxels[handle])
    {
      voxelRB->applyCentralImpulse(rayDirection.normalize() * force);
      //       voxelRB->applyCentralImpulse(btVector3(1.0, 0.0, 0.0) * force);
      voxelRB->activate(true);
    }
  }
}

void PhysicsEngine::Init()
{
  mObjectWorld.mDynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));
  mVoxelWorld.mDynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));
  mVoxelWorld.mDynamicsWorld->setDebugDrawer((btIDebugDraw *)mDebugDrawer);
  mObjectWorld.mDynamicsWorld->setDebugDrawer((btIDebugDraw *)mDebugDrawer);
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
  static u32 dontCollide = 1;
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
    //     mVoxelWorld.mDynamicsWorld->addRigidBody(rigidBody, dontCollide, 0xffff ^ dontCollide);
    mVoxelWorld.mDynamicsWorld->addRigidBody(rigidBody);

    mVoxels[handle].emplace_back(rigidBody);
    neighbors.emplace(key, rigidBody);
  }
  dontCollide <<= 1;
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

  std::unordered_set<glm::uvec3> createdConstraints;
  u32 originalConstraintCount = 0;
  u32 newConstraintCount = 0;
  for (auto &[key, voxel] : vMesh->mVoxels)
  {

    auto size = voxel.mDimensions;
    // TODO: don't create double constraints
    for (const auto &n : voxel.mNeighbors)
    {
      originalConstraintCount++;
      glm::uvec3 voxelPairKey = key + n;
      if (createdConstraints.find(voxelPairKey) != createdConstraints.end())
      {
        continue;
      }
      newConstraintCount++;
      createdConstraints.emplace(voxelPairKey);
      glm::vec3 ap(glm::abs(glm::vec3(n) - glm::vec3(key)));
      glm::vec3 bp(-ap);
      auto *voxelRB = neighbors[key];
      auto *neighbor = neighbors[n];
      auto a = voxelRB->getWorldTransform(); // btTransform::getIdentity();
      a.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
      // a.setOrigin(btVector3(ap.x, ap.y, ap.z));
      auto b = neighbor->getWorldTransform(); // btTransform::getIdentity();
      b.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
      // b.setOrigin(btVector3(bp.x, bp.y, bp.z));
      auto *constraint = new btGeneric6DofSpringConstraint(*voxelRB, *neighbor, a, b, true);
      auto lower = -size + ap;
      auto upper = size - ap;
      //       constraint->setLinearLowerLimit(ToBullet(lower));
      //       constraint->setLinearUpperLimit(ToBullet(upper));
      constraint->setLimit(0, -1.0, 1.0);
      constraint->setLimit(1, -1.0, 1.0);
      constraint->setLimit(2, -1.0, 1.0);
      //       if (ap.x != 0.0f)
      //       {
      //         constraint->setLimit(0, 0.0, 0.0);
      //       }
      //       if (ap.y != 0.0f)
      //       {
      //         constraint->setLimit(1, 0.0, 0.0);
      //       }
      //       if (ap.z != 0.0f)
      //       {
      //         constraint->setLimit(2, 0.0, 0.0);
      //       }
      mVoxelWorld.mDynamicsWorld->addConstraint(constraint, false);
    }
  }
  return collisionShape;
}

void PhysicsEngine::AdjustVoxelSizeFromImpulse(
    VoxObj::Voxel *voxel, f32 impulse, u32 modifiedDimm, u32 unchangedDimm0, u32 unchangedDimm1)
{
  f32 springForce = -1.0f * voxel->mDimensions[modifiedDimm];
  // apply the impulse then
  // solve for the new values of the undisturbed dimensions
  if (impulse > springForce)
  {
    springForce += impulse;
    f32 volume = glm::compMul(voxel->mDimensions);
    voxel->mDimensions[modifiedDimm] = glm::abs(springForce / -2.0f);

    volume = volume / voxel->mDimensions[modifiedDimm];
    f32 volume2 = glm::compMul(voxel->mDimensions);
    int foo = 0;

    voxel->mDimensions[unchangedDimm0] = sqrt(volume);
    voxel->mDimensions[unchangedDimm1] = sqrt(volume);
    voxel->mDimensions = glm::abs(voxel->mDimensions);
  }
}

} // namespace Physics

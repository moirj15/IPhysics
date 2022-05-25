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
  mObjectSettings.clear();
  mObjectHandles.clear();
  Init();
}

void PhysicsEngine::Update(f32 t)
{
  mObjectWorld.mDynamicsWorld->stepSimulation(t, 10);
  mVoxelWorld.mDynamicsWorld->stepSimulation(t, 10);
  // TODO: add a bool to make this optional
  auto &voxelDispatcher = mVoxelWorld.mCollisionDispatcher;
  if (mSettings.mEnableExtension) {
    s32 numManifolds = voxelDispatcher->getNumManifolds();
    for (s32 i = 0; i < numManifolds; i++) {
      auto *contactManifold = voxelDispatcher->getManifoldByIndexInternal(i);
      s32 numContacts = contactManifold->getNumContacts();
      for (s32 j = 0; j < numContacts; j++) {
        auto &contactPoint = contactManifold->getContactPoint(j);
        f32 impulse = contactPoint.getAppliedImpulse();
        auto *voxelRB = contactManifold->getBody1();
        auto *voxel = (objs::Voxel *)voxelRB->getUserPointer();
        // TODO: store the mass of the voxel, since we can't get this from bullet
        f32 density = glm::compMul(voxel->dimmensions) * 1.0f;
        //const f32 impulseThreshold = density; // * 10.0f;
        const f32 impulseThreshold = 0.01; // * 10.0f;
        if (impulse > impulseThreshold) {
          // Use the contactPoint's B normal to determine which voxel face to adjust
          auto contactNormal = ToGLM(contactPoint.m_normalWorldOnB);
          //         contactNormal *= impulse;
          //         printf("impulse * t %f\n", impulse * t * 0.01f);
          for (u32 v = 0; v < voxel->bezierCurves.size(); v++) {
            auto &bezierCurve = voxel->bezierCurves[v];
            // TODO: compare these two methods of modifying the bezier curves
            //           for (auto &cp : bezierCurve.mControlPoints)
            //           {
            //             cp += impulseDirection;
            //           }
            auto &firstCP = bezierCurve.controlPoints[0];
            auto &secondCP = bezierCurve.controlPoints[bezierCurve.controlPoints.size() - 1];
            auto firstCPNormal = glm::normalize(firstCP - voxel->position);
            auto secondCPNormal = glm::normalize(secondCP - voxel->position);
            // check if the normalized control point and the contact normal point in the same
            // direction
            if (glm::dot(firstCPNormal, contactNormal) > 0.0f) {
              firstCP += (-contactNormal * impulse) * t; // * t;
              bezierCurve.needsUpdate = true;
            }
            if (glm::dot(secondCPNormal, contactNormal) > 0.0f) {
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
#if 0
  if (mSettings.mEnableExtension) {
    for (const auto &[handle, voxelRBs] : mSoftVoxels) {
      auto &position = mObjectSettings[handle].position;
      for (auto &voxelRB : voxelRBs) {


        auto *voxel = (objs::Voxel *)voxelRB->getUserPointer();
        for (auto &curve : voxel->bezierCurves) {
          for (u32 controlPointIndex = 0; controlPointIndex < curve.controlPoints.size(); controlPointIndex++) {
            u32 nodeIndex = curve.controlPointBulletNodeIndices[controlPointIndex];
            curve.controlPoints[controlPointIndex] = ToGLM(voxelRB->m_nodes[nodeIndex].m_x);
          }
        }
        const auto &origin = voxelRB->getWorldTransform().getOrigin();
        glm::vec3 voxelCurrPosition(origin.x(), origin.y(), origin.z());
        auto orignalPosition = voxel->position;
        voxel->position = voxelCurrPosition;
        auto toNewPosition = voxel->position - orignalPosition;
        // TODO: may have to move this to before the dimensions of the voxel are changed.
        // voxel->UpdateBezierCurves(toNewPosition);
        voxel->relativePositionDelta = (voxel->position - position) - voxel->positionRelativeToCenter;
        voxel->positionRelativeToCenter = voxel->position - position;
      }
    }
  } else {
#endif
    for (const auto &[handle, voxelRBs] : mVoxels) {
      auto &position = mObjectSettings[handle].position;
      for (auto &voxelRB : voxelRBs) {

        const auto &origin = voxelRB->getWorldTransform().getOrigin();
        glm::vec3 voxelCurrPosition(origin.x(), origin.y(), origin.z());
        auto *voxel = (objs::Voxel *)voxelRB->getUserPointer();
        auto orignalPosition = voxel->position;
        voxel->position = voxelCurrPosition;
        auto toNewPosition = voxel->position - orignalPosition;
        // TODO: may have to move this to before the dimensions of the voxel are changed.
        // voxel->UpdateBezierCurves(toNewPosition);
        voxel->relativePositionDelta = (voxel->position - position) - voxel->positionRelativeToCenter;
        voxel->positionRelativeToCenter = voxel->position - position;
      }
    }
#if 0
  }
#endif
  for (const auto handle : mObjectHandles) {
    // Update our object position in its settings, so we can render it in the right spot
    auto &objectPosition = mObjectSettings[handle].position;
    auto *rb = (btRigidBody *)mObjects[handle].get();
    const auto &transform = rb->getWorldTransform();
    const auto &position = transform.getOrigin();
    objectPosition = ToGLM(position);
    auto *collisionShape = (btCompoundShape *)rb->getCollisionShape();
    for (s32 i = 0; i < collisionShape->getNumChildShapes(); i++) {
      auto *childCollisionShape = (btBoxShape *)collisionShape->getChildShape(i);
      auto *voxel = (objs::Voxel *)childCollisionShape->getUserPointer();
      auto tmp = collisionShape->getChildTransform(i);
      btTransform newTransform(
          btQuaternion(), btVector3(voxel->positionRelativeToCenter.x, voxel->positionRelativeToCenter.y,
                              voxel->positionRelativeToCenter.z));
      collisionShape->updateChildTransform(i, newTransform);
    }
  }
  mObjectWorld.mDynamicsWorld->debugDrawWorld();
  mVoxelWorld.mDynamicsWorld->debugDrawWorld();
}

void PhysicsEngine::SubmitObject(MeshHandle handle, const SceneMember &sceneMember)
{
  mObjectHandles.emplace_back(handle);
  mObjectSettings[handle] = sceneMember;
  // mObjectPositions.emplace(handle, glm::vec3(0.0f, 0.0f, 0.0f));
  auto *collisionShape = AddVoxels(handle);
  AddObject(handle, collisionShape);
}

void PhysicsEngine::UpdateObject(MeshHandle handle, const glm::vec3 &position)
{
  mObjectSettings[handle].position = position;
  btTransform transform;
  transform.setOrigin(ToBullet(position));
  mObjects[handle]->setWorldTransform(transform);
}

void PhysicsEngine::CastRayWithForce(
    const glm::vec3 &rayStartNDC, const glm::vec3 &rayEndNDC, const glm::mat4 &NDCToWorldSpace, f32 force)
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
  if (rayCallback.hasHit()) {
    auto *rigidBody = (btRigidBody *)rayCallback.m_collisionObject;
    rigidBody->activate(true);
    rigidBody->applyCentralImpulse(rayDirection.normalize() * force);
    //     rigidBody->applyCentralImpulse(btVector3(1.0, 0.0, 0.0) * force);

    auto handle = (MeshHandle)rigidBody->getUserIndex();
#if 0
    if (mSettings.mEnableExtension) {
      for (auto &voxelRB : mSoftVoxels[handle]) {
        voxelRB->addForce(rayDirection.normalize() * force);
        //       voxelRB->applyCentralImpulse(btVector3(1.0, 0.0, 0.0) * force);
        voxelRB->activate(true);
      }
    } else {
#endif
      for (auto &voxelRB : mVoxels[handle]) {
        voxelRB->applyCentralImpulse(rayDirection.normalize() * force);
        //       voxelRB->applyCentralImpulse(btVector3(1.0, 0.0, 0.0) * force);
        voxelRB->activate(true);
      }
#if 0
    }
#endif
  }
}

void PhysicsEngine::Init()
{
  mObjectWorld.mDynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));
  mVoxelWorld.mDynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));
  mVoxelWorld.mDynamicsWorld->setDebugDrawer((btIDebugDraw *)mDebugDrawer.get());
  mObjectWorld.mDynamicsWorld->setDebugDrawer((btIDebugDraw *)mDebugDrawer.get());

  // All initialization for soft bodies was takin from the bullet3 soft-body demo
  // https://github.com/bulletphysics/bullet3/blob/master/examples/SoftDemo/SoftDemo.cpp
  auto collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
  auto collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);

  mSoftBodyWorldInfo.m_dispatcher = collisionDispatcher;
  btVector3 worldMin(-1000, -1000, -1000);
  btVector3 worldMax(1000, 1000, 1000);

  const int maxProxies = 32766;
  auto broadPhase = new btAxisSweep3(worldMin, worldMax, maxProxies);
  mSoftBodyWorldInfo.m_broadphase = broadPhase;
  mSoftBodyWorldInfo.m_gravity.setValue(0, 0, 0);
  mSoftBodyWorldInfo.m_sparsesdf.Initialize();
  auto constraintSolver = new btSequentialImpulseConstraintSolver();
  mSoftBodyWorld =
      new btSoftRigidDynamicsWorld(collisionDispatcher, broadPhase, constraintSolver, collisionConfiguration);
  mSoftBodyWorld->setDebugDrawer((btIDebugDraw*)mDebugDrawer.get());
  //mSoftBodyWorld->setGravity(btVector3(0, 0, 0));

  // mSoftBodyWorldInfo.m_broadphase = mVoxelWorld.mOverlappingPairCache.get();
  // mSoftBodyWorldInfo.m_dispatcher = mVoxelWorld.mCollisionDispatcher.get();
  // mSoftBodyWorld =
  //    new btSoftRigidDynamicsWorld(mVoxelWorld.mCollisionDispatcher.get(), mVoxelWorld.mOverlappingPairCache.get(),
  //        mVoxelWorld.mSolver.get(), mVoxelWorld.mCollisionConfig.get()/*, new btDefaultSoftBodySolver()*/);
}

void PhysicsEngine::AddObject(MeshHandle handle, btCompoundShape *collisionShape)
{
  auto &position = mObjectSettings[handle].position;

  btTransform transform = btTransform::getIdentity();
  transform.setOrigin(ToBullet(position));

  auto *motionState = new btDefaultMotionState(transform);

  btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(1.0f, motionState, collisionShape);

  auto rigidBody = new btRigidBody(rigidBodyInfo);
  rigidBody->setUserIndex((s32)handle);

  mObjectWorld.mDynamicsWorld->addRigidBody(rigidBody);
  mObjects.emplace(handle, rigidBody);
}

btCompoundShape *PhysicsEngine::AddVoxels(MeshHandle handle)
{
  auto *vMesh = mMeshManager->GetVoxelMesh(handle);
  auto &position = mObjectSettings[handle].position;
  mVoxels.emplace(handle, std::vector<std::unique_ptr<btRigidBody>>());
  std::unordered_map<glm::uvec3, btRigidBody *> voxelNeighbors;
  auto *collisionShape = new btCompoundShape(true, (int)vMesh->voxels.size());
  static u32 dontCollide = 1;

  // Loop through the voxels and create the corresponding bullet physics objects
  for (auto &[key, voxel] : vMesh->voxels) {
    voxel.dimmensions = glm::vec3(vMesh->initialVoxelSize);
    // Update the initial voxel position
    voxel.position += position;
    const f32 scale = voxel.size.x;
    #if 0
    const btVector3 hull[] = {
        ToBullet(voxel.position + (scale * glm::vec3(1.0f, 1.0f, 1.0f))),
        ToBullet(voxel.position + (scale * glm::vec3(1.0f, 1.0f, -1.0f))),
        ToBullet(voxel.position + (scale * glm::vec3(1.0f, -1.0f, 1.0f))),
        ToBullet(voxel.position + (scale * glm::vec3(1.0f, -1.0f, -1.0f))),
        ToBullet(voxel.position + (scale * glm::vec3(-1.0f, 1.0f, 1.0f))),
        ToBullet(voxel.position + (scale * glm::vec3(-1.0f, 1.0f, -1.0f))),
        ToBullet(voxel.position + (scale * glm::vec3(-1.0f, -1.0f, 1.0f))),
        ToBullet(voxel.position + (scale * glm::vec3(-1.0f, -1.0f, -1.0f))),
    };
    #if 0
    for (auto &bezierCurve : voxel.bezierCurves) {
      for (const auto &controlPoint : bezierCurve.controlPoints) {
        bezierCurve.controlPointBulletNodeIndices.push_back(hull.size());
        hull.emplace_back(ToBullet(controlPoint + voxel.position));
      }
    }
    #endif
    u32 nodeCount = 8;
    auto *softBody = btSoftBodyHelpers::CreateFromConvexHull(mSoftBodyWorldInfo, hull, nodeCount);
    for (auto &bezierCurve : voxel.bezierCurves) {
      for (const auto &controlPoint : bezierCurve.controlPoints) {
        bezierCurve.controlPointBulletNodeIndices.push_back(nodeCount);
        nodeCount++;
        softBody->appendNode(ToBullet(controlPoint + voxel.position), 1.0);
      }
    }
    softBody->setUserPointer((void *)&vMesh->voxels[key]);
    softBody->setWorldTransform(btTransform(btQuaternion(), ToBullet(voxel.position)));
    softBody->generateBendingConstraints(2, softBody->m_materials[0]);
    mSoftBodyWorld->addSoftBody(softBody);
    mSoftVoxels[handle].emplace_back(softBody);
    #endif
    // TODO rest
    // Do all the bullet object creation stuff
    btCollisionShape *voxelCollisionShape = new btBoxShape(ToBullet(voxel.dimmensions / 2.0f));
    // Add the voxel to the btCompundShape
    voxelCollisionShape->setUserPointer((void *)&vMesh->voxels[key]);

    f32 mass = 1.0f;
    btVector3 localInteria(0.0f, 0.0f, 0.0f);
    voxelCollisionShape->calculateLocalInertia(1.0f, localInteria);

    auto *motionState = new btDefaultMotionState(btTransform(btQuaternion(), ToBullet(voxel.position)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(
        1.0f, motionState, voxelCollisionShape, btVector3(0.0f, 0.0f, 0.0f));

    auto rigidBody = new btRigidBody(rigidBodyInfo);
    rigidBody->setUserIndex((int)handle);

    // set the user index so we can get this voxel at a later time
    rigidBody->setUserIndex((int)handle);
    rigidBody->setUserIndex2((int)mVoxels[handle].size());
    rigidBody->setUserPointer((void *)&vMesh->voxels[key]);

    collisionShape->addChildShape(rigidBody->getWorldTransform(), voxelCollisionShape);

    // Add the object to the voxel world so it will only interact with other voxels
    //     mVoxelWorld.mDynamicsWorld->addRigidBody(rigidBody, dontCollide, 0xffff ^ dontCollide);
    mVoxelWorld.mDynamicsWorld->addRigidBody(rigidBody);

    mVoxels[handle].emplace_back(rigidBody);
    voxelNeighbors.emplace(key, rigidBody);
  }
  dontCollide <<= 1;
  std::vector<f32> masses(vMesh->voxels.size(), 1.0f);
  btTransform principal;
  btVector3 inertia;
  collisionShape->calculatePrincipalAxisTransform(masses.data(), principal, inertia);
  for (s32 i = 0; i < collisionShape->getNumChildShapes(); i++) {
    collisionShape->getChildTransform(i) *= principal.inverse();
    auto *child = collisionShape->getChildShape(i);
    auto *voxel = (objs::Voxel *)child->getUserPointer();
    voxel->positionRelativeToCenter = voxel->position - position;
  }

  std::unordered_set<glm::uvec3> createdConstraints;
  u32 originalConstraintCount = 0;
  u32 newConstraintCount = 0;
  for (auto &[key, voxel] : vMesh->voxels) {
    auto size = voxel.dimmensions;
    // TODO: don't create double constraints
    for (u32 i = 0; i < 6; i++) {
      if (!voxel.neighbors[i]) {
        continue;
      }
      // TODO: may need to put this before the check
      originalConstraintCount++;
      glm::uvec3 voxelPairKey = glm::uvec3(glm::ivec3(key) + objs::Voxel::NEIGHBOR_OFFSETS[i]);
      if (createdConstraints.contains(voxelPairKey)) {
        continue;
      }
      newConstraintCount++;
      createdConstraints.emplace(voxelPairKey);

      glm::vec3 ap(glm::abs(glm::vec3(objs::Voxel::NEIGHBOR_OFFSETS[i]) - glm::vec3(key)));
      glm::vec3 bp(-ap);
      auto *voxelRB = voxelNeighbors[key];
      auto *neighbor = voxelNeighbors[voxelPairKey];
      auto a = voxelRB->getWorldTransform(); // btTransform::getIdentity();
      a.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
      // a.setOrigin(btVector3(ap.x, ap.y, ap.z));
      auto b = neighbor->getWorldTransform(); // btTransform::getIdentity();
      b.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
      // b.setOrigin(btVector3(bp.x, bp.y, bp.z));

      // Prevent the voxels from sliding too far from their neighbors
      auto *constraint = new btGeneric6DofSpringConstraint(*voxelRB, *neighbor, a, b, true);
      auto lower = -size + ap;
      auto upper = size - ap;
      constraint->setLinearLowerLimit(ToBullet(lower));
      constraint->setLinearUpperLimit(ToBullet(upper));

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
    objs::Voxel *voxel, f32 impulse, u32 modifiedDimm, u32 unchangedDimm0, u32 unchangedDimm1)
{
  f32 springForce = -1.0f * voxel->dimmensions[modifiedDimm];
  // apply the impulse then
  // solve for the new values of the undisturbed dimensions
  if (impulse > springForce) {
    springForce += impulse;
    f32 volume = glm::compMul(voxel->dimmensions);
    voxel->dimmensions[modifiedDimm] = glm::abs(springForce / -2.0f);

    volume = volume / voxel->dimmensions[modifiedDimm];
    f32 volume2 = glm::compMul(voxel->dimmensions);
    int foo = 0;

    voxel->dimmensions[unchangedDimm0] = sqrt(volume);
    voxel->dimmensions[unchangedDimm1] = sqrt(volume);
    voxel->dimmensions = glm::abs(voxel->dimmensions);
  }
}

} // namespace Physics

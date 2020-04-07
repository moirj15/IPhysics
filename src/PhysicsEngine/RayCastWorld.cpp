#include "RayCastWorld.h"

#include "../utils/MathCasts.h"

// void RayCastWorld::AddBoxes(std::vector<Box> &boxes)
// {
//   //   auto *voxelCollisionShape = new btBoxShape(boxes.back().mExtents / 2.0f);
//   //   for (const auto &box : boxes)
//   //   {
//   //     btTransform transform;
//   //     transform.setIdentity();
//   //     transform.setOrigin(box.mPos);
//   //     //     auto *motionState = new btDefaultMotionState(btTransform(btQuaternion(),
//   box.mPos));
//   //     auto *motionState = new btDefaultMotionState(transform);
//   //
//   //     btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(
//   //         0.0f, motionState, voxelCollisionShape, btVector3(0.0f, 0.0f, 0.0f));
//   //
//   //     mRigidBodies.emplace_back(new btRigidBody(rigidBodyInfo));
//   //
//   //     mCollisionWorld->addCollisionObject(mRigidBodies.back().get());
//   //   }
// }
//
// std::unordered_set<glm::vec3> RayCastWorld::CastRay(const Ray &firstRay, const Ray &secondRay)
// {
//   //   std::unordered_set<glm::vec3> intersections;
//   //
//   //   btCollisionWorld::ClosestRayResultCallback rayCastResult(firstRay.mOrigin,
//   //   firstRay.mDirection); mCollisionWorld->rayTest(firstRay.mOrigin, firstRay.mDirection,
//   //   rayCastResult); if (rayCastResult.hasHit())
//   //   {
//   //     intersections.emplace(ToGLM(rayCastResult.m_hitPointWorld));
//   //   }
//   //   btCollisionWorld::ClosestRayResultCallback foo(firstRay.mOrigin, -firstRay.mDirection);
//   //   mCollisionWorld->rayTest(firstRay.mOrigin, -firstRay.mDirection, foo);
//   //   if (foo.hasHit())
//   //   {
//   //     intersections.emplace(ToGLM(foo.m_hitPointWorld));
//   //   }
//   //
//   //   return intersections;
//   return {};
// }

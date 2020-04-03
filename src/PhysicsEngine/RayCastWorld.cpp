#include "RayCastWorld.h"

void RayCastWorld::AddBoxes(std::vector<Box> &boxes)
{
  for (const auto &box : boxes)
  {
    btCollisionShape *voxelCollisionShape = new btBoxShape(box.mExtents);
    auto *motionState = new btDefaultMotionState(btTransform(btQuaternion(), box.mOrigin));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(
        0.0f, motionState, voxelCollisionShape, btVector3(0.0f, 0.0f, 0.0f));

    mRigidBodies.emplace_back(new btRigidBody(rigidBodyInfo));
  }
}

std::vector<glm::vec3> RayCastWorld::CastRay(const Ray &ray)
{
  std::vector<glm::vec3> intersections;
  btCollisionWorld::AllHitsRayResultCallback allResults(ray.mOrigin, ray.mDirection);
  mCollisionWorld->rayTest(ray.mOrigin, ray.mDirection, allResults);
  for (s32 i = 0; i < allResults.m_hitPointWorld.size(); i++)
  {
    auto intersection = allResults.m_hitPointWorld[i];
    intersections.emplace_back(intersection.x(), intersection.y(), intersection.z());
  }

  return intersections;
}

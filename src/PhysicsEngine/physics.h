#pragma once

#include "../common.h"

#include <memory>
#include <reactphysics3d.h>
#include <vector>

class Object;
class Voxel;

namespace pye
{

struct CollisionCallback final : public rp3d::CollisionCallback
{
  std::vector<rp3d::CollisionCallback::CollisionCallbackInfo> mCallbackInfo;
  inline void notifyContact(const CollisionCallbackInfo &collisionCallbackInfo) override
  {
    mCallbackInfo.push_back(collisionCallbackInfo);
  }

  inline void Clear()
  {
    mCallbackInfo.clear();
  }
};

class PhysicsEngine
{
  std::unique_ptr<rp3d::DynamicsWorld> mObjectWorld;
  std::unique_ptr<rp3d::DynamicsWorld> mVoxelWorld;
  std::unique_ptr<CollisionCallback> mObjectWorldCallback;
  std::unique_ptr<CollisionCallback> mVoxelWorldCallback;

  bool mCollisionOccuring;

public:
  PhysicsEngine() :
      mObjectWorld(new rp3d::DynamicsWorld(rp3d::Vector3(0.0f, 0.0f, 0.0f))),
      mVoxelWorld(new rp3d::DynamicsWorld(rp3d::Vector3(0.0f, 0.0f, 0.0f))),
      mObjectWorldCallback(new CollisionCallback), mVoxelWorldCallback(new CollisionCallback),
      mCollisionOccuring(false)
  {
  }

  void Update(f32 t);

  void SubmitObject(Object *object);

  inline bool CollisionOccuring() const
  {
    return mCollisionOccuring;
  }

private:
  void CollectVoxelsWithBadPositions();
  void CorrectVoxelPositions();
};

} // namespace pye
#include "physics.h"

namespace pye
{

void PhysicsEngine::Update(f32 t)
{
  mObjectWorldCallback->Clear();
  mObjectWorld->update(t);
  mObjectWorld->testCollision(mObjectWorldCallback.get());
  mCollisionOccuring = !mObjectWorldCallback->mCallbackInfo.empty();
  if (mCollisionOccuring)
  {
    mVoxelWorld->update(t);
    mVoxelWorld->testCollision(mVoxelWorldCallback.get());
    // TODO: prevent voxels from drifting apart
  }
}
void PhysicsEngine::CorrectVoxelPositions()
{
}

} // namespace pye

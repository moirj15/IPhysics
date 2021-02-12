
// input data
struct Edge 
{
  int a;
  int b;
};

struct Aabb
{
  float3 minCorner;
  float3 maxCorner;
};

cbuffer Constants
{
  Aabb meshAABB;
  int3 dimmensions;
};

Buffer<float3> vertices;

// output data
struct VertexVoxelInfo
{
  int3 voxel;
  // 00 - neither are in the voxel, but the edge is
  // 01 - a is in the voxel
  // 10 - b is in the voxel
  // 11 - both are in the voxel
  int endpointInVoxel; 
};


// Output buffers
RWBuffer<int3> vertexOwners;
// {
//   // Each element points to the voxel that owns the vertex with the same index
//   int3 verticesOwningVoxel[];
// };

RWBuffer<bool> existingVoxels;

// Dispatch on the vertices
[numthreads(1, 1, 1)]
void CSMain(int3 threadID : SV_DispatchThreadID) {
  const float3 voxelSize = 1.0f / float3(dimmensions);
  const float3 vertex = vertices[threadID.x];
  // Get the number of voxels by adjusting the maxCorner of the aabb to be based off the origin then divide by the provided voxel size
  const int3 numVoxels = (meshAABB.maxCorner + (-meshAABB.minCorner)) / voxelSize;
  for (int x = 0; x < numVoxels.x; x++) {
    for (int y = 0; y < numVoxels.y; y++) {
      for (int z = 0; z < numVoxels.z; z++) {
        float3 offset = float3(float(x) * voxelSize.x, float(y) * voxelSize.y, float(z) * voxelSize.z);
        // Calculate the AABB that will act as the voxel
        Aabb voxel = {meshAABB.minCorner + offset, meshAABB.minCorner + offset + voxelSize};
        // Calculate the voxel's center
        float3 voxelCenter = voxel.minCorner + ((voxel.maxCorner - voxel.minCorner) / 2.0);
        // Check if the current voxel contains the vertex and update the existingVoxel array accordingly
        bool voxelContainsVertex = all((vertex >= voxel.minCorner) && (vertex <= voxel.maxCorner));
        if (voxelContainsVertex) {
          existingVoxels[(z * numVoxels.z) + (y * numVoxels.y) + x] = voxelContainsVertex; // Use a bitwise-or so false is never assigned
          // Store the voxel that the vertex belongs to in the OutputData
          vertexOwners[threadID.x] = int3(x, y, z);
        }
      }
    }
  }
}

#pragma once

#include "../../common.h"
#include "../renderer/mesh.h"

#include <glm/vec3.hpp>
#include <unordered_map>
#include <vector>

// struct Face
// {
//   std::vector<u32> mEdges;
// };

struct Edge
{
  u32 mStartVert = 0;
  u32 mEndVert = 0;
  //   Face mFaces[2];
  //   std::vector<u32> mPreviousEdges;
  //   std::vector<u32> mNextEdges;

  Edge() = default;

  Edge(u32 startVert, u32 endVert) : mStartVert(startVert), mEndVert(endVert)
  {
  }
  Edge(const Edge &e) : mStartVert(e.mStartVert), mEndVert(e.mEndVert)
  {
  }

  bool operator==(const Edge &e) const
  {
    return (mStartVert == e.mStartVert) && (mEndVert == e.mEndVert);
  }

  //   explicit Edge(const Edge &e) :
  //       mStartVert(e.mStartVert), mEndVert(e.mEndVert), mFaces{e.mFaces[0], e.mFaces[1]},
  //       mPreviousEdges(e.mPreviousEdges), mNextEdges(e.mNextEdges)
  //   {
  //   }
};

namespace std
{
template<>
struct hash<Edge>
{
  size_t operator()(const Edge &e) const
  {
    return hash<u32>()(e.mStartVert) + hash<u32>()(e.mEndVert);
  }
};
} // namespace std

// class EdgeMesh
// {
//   Mesh *mMesh;
//   std::unordered_map<u32, Edge> mEdges;
//
// public:
//   explicit EdgeMesh(Mesh *mesh) : mMesh(mesh)
//   {
//     //     PopulateFromMesh();
//   }
//   inline void Insert(const Edge &e)
//   {
//     mEdges.emplace(e.mStartVert, e);
//   }
//   NODISCARD inline const Edge &GetEdge(const u32 startVert)
//   {
//     return mEdges[startVert];
//   }
//
// private:
//   void PopulateFromMesh();
// };
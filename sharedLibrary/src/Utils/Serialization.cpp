#include <Renderer/Mesh.h>
#include <Utils/Serialization.h>
#include <VoxelObjects/VoxelMesh.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace glm
{

void to_json(nlohmann::json &j, const uvec3 &v)
{
  j = json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}
void from_json(const nlohmann::json &j, uvec3 &v)
{
  j.at("x").get_to(v.x);
  j.at("y").get_to(v.y);
  j.at("z").get_to(v.z);
}

void to_json(nlohmann::json &j, const vec3 &v)
{
  j = json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}
void from_json(const nlohmann::json &j, vec3 &v)
{
  j.at("x").get_to(v.x);
  j.at("y").get_to(v.y);
  j.at("z").get_to(v.z);
}

} // namespace glm

void to_json(nlohmann::json &j, const BezierCurve &b)
{
  j = json{{"ControlPoints", b.mControlPoints}, {"FirstT", b.mFirstT}, {"SecondT", b.mSecondT},
           {"EffectedPoitns", b.mEffectedPoints}};
}
void from_json(const nlohmann::json &j, BezierCurve &b)
{
  j.at("ControlPoints").get_to(b.mControlPoints);
  j.at("FirstT").get_to( b.mFirstT);
  j.at("SecondT").get_to(b.mSecondT);
  j.at("EffectedPoitns").get_to( b.mEffectedPoints);
}


namespace VoxObj
{

void to_json(nlohmann::json &j, const Voxel &v)
{
  j = json{
      {"Dimensions", v.mDimensions}, {"Position", v.mPosition}, {"BezierCurves", v.mBezierCurves}};
}
void from_json(const nlohmann::json &j, Voxel &v)
{
  j.at("Dimensions").get_to(v.mDimensions);
  j.at("Position").get_to(v.mPosition);
  j.at("BezierCurves").get_to(v.mBezierCurves);
}

} // namespace VoxObj

namespace Utils
{
/*
File Structure:

int : number of voxels
int : number of mesh verts
int : number of mesh indices
int : number of mesh normals
glm::uvec3 : voxel mesh extents voxel space
glm::vec3 : voxel mesh extents object space
mesh contents
voxels
 */

void to_json(json &j, const glm::vec3 &v)
{
  j = json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}
void from_json(const json &j, glm::vec3 &v)
{
  j.at("x").get_to(v.x);
  j.at("y").get_to(v.y);
  j.at("z").get_to(v.z);
}

void Serialize(VoxObj::VoxelMesh *voxelMesh, const std::string &path)
{
  json output;
  output["ExtentsVoxelSpace"] = voxelMesh->mExtentsVoxelSpace;
  output["ExtentsObjectSpace"] = voxelMesh->mExtentsVoxelSpace;
  output["InitialVoxelSize"] = voxelMesh->mExtentsVoxelSpace;
  auto *mesh = voxelMesh->mMesh;
  json meshJson;
  meshJson["Vertices"] = mesh->mVertices.GetBuffer();
  meshJson["Normals"] = mesh->mNormals.GetBuffer();
  meshJson["Indices"] = mesh->mIndices;
  output["Mesh"] = meshJson;
  std::vector<std::pair<glm::uvec3, VoxObj::Voxel>> voxels;
  for (const auto &pair : voxelMesh->mVoxels)
  {
    voxels.emplace_back(pair);
  }
  output["Voxels"] = voxels;
//  output.emplace_back(voxelJsons);
  std::cout << output << std::endl;

  // TODO: This code is horrible, clean it up
  // void Serialize(VoxObj::VoxelMesh *voxelMesh, const std::string &path)
  //{
  //  std::ofstream file(path, std::ios::trunc | std::ios::out);
  //  auto *mesh = voxelMesh->GetMesh();
  //  file << "[Vertex Count]\n" << mesh->mVertices.BufferSize() << "\n";
  //  file << "[Index Count]\n" << mesh->mIndices.size() << "\n";
  //  file << "[Normal Count]\n" << mesh->mNormals.BufferSize() << "\n";
  //  file << "[Voxel Count]\n" << voxelMesh->GetVoxelCount() << "\n";
  //
  //  const auto extentsVoxelSpace = voxelMesh->GetExtentsVoxelSpace();
  //  file << "[Extents (Voxel Space)]\n"
  //       << extentsVoxelSpace.x << " " << extentsVoxelSpace.y << " " << extentsVoxelSpace.z <<
  //       "\n";
  //
  //  const auto extentsObjectSpace = voxelMesh->GetExtentsObjectSpace();
  //  file << "[Extents (Object Space)]\n"
  //       << extentsObjectSpace.x << " " << extentsObjectSpace.y << " " << extentsObjectSpace.z
  //       << "\n";
  //
  //  file << "[Initial Voxel Size]\n"
  //       << voxelMesh->GetInitialVoxelSize().x << " " << voxelMesh->GetInitialVoxelSize().y << " "
  //       << voxelMesh->GetInitialVoxelSize().z << " "
  //       << "\n";
  //
  //  file << "[Vertices]\n";
  //  for (const auto &v : mesh->mVertices.GetBuffer())
  //  {
  //    file << v << " ";
  //  }
  //  file << "\n[Indecies]\n";
  //  for (const auto &i : mesh->mIndices)
  //  {
  //    file << i << " ";
  //  }
  //  file << "\n[Normals]\n";
  //  for (const auto &n : mesh->mNormals.GetBuffer())
  //  {
  //    file << n << " ";
  //  }
  //
  //  // Begin serializing the voxels
  //  file << "\n[Voxels]\n";
  //  for (const auto &[key, value] : voxelMesh->mVoxels)
  //  {
  //    file << "[Key]\n" << key.x << " " << key.y << " " << key.z << "\n";
  //    file << "[Dimensions]\n"
  //         << value.mDimensions.x << " " << value.mDimensions.y << " " << value.mDimensions.z <<
  //         "\n";
  //    file << "[Position]\n"
  //         << value.mPosition.x << " " << value.mPosition.y << " " << value.mPosition.z << "\n";
  //    file << "[Neighbor Count]\n" << value.mNeighbors.size() << "\n";
  //    for (const auto &n : value.mNeighbors)
  //    {
  //      file << n.x << " " << n.y << " " << n.z << " ";
  //    }
  //    file << "\n";
  //    file << "[Contained Mesh Vertex Count]\n" << value.mMeshVertices.size() << "\n";
  //    for (const auto &v : value.mMeshVertices)
  //    {
  //      file << v << " ";
  //    }
  //    if (!value.mMeshVertices.empty())
  //    {
  //      file << "\n";
  //    }
  //    // Start serializing the bezier curves
  //    file << "[Bezier Curve Count]\n" << value.mBezierCurves.size() << "\n";
  //    for (const auto &b : value.mBezierCurves)
  //    {
  //      file << "[TStart]\n" << b.mFirstT << "\n";
  //      file << "[TEnd]\n" << b.mSecondT << "\n";
  //      file << "[Control Point Count]\n" << b.mControlPoints.size() << "\n";
  //      file << "[Control Points]\n";
  //      for (const auto &cp : b.mControlPoints)
  //      {
  //        file << cp.x << " " << cp.y << " " << cp.z << " ";
  //      }
  //      file << "\n";
  //      file << "[Effected point count]\n" << b.mEffectedPoints.size() << "\n";
  //      file << "[Effected points]\n";
  //      for (const auto &e : b.mEffectedPoints)
  //      {
  //        file << e << " ";
  //      }
  //
  //      file << "\n";
  //    }
  //  }
  //}
}

VoxObj::VoxelMesh *DeSerialize(const std::string &path)
{
  std::fstream file(path, std::ios::in);
  std::cout << file.good() << '\n';
  u64 vertCount = 0;
  u64 indexCount = 0;
  u64 normalCount = 0;
  u64 voxelCount = 0;

  // Load in the number of mesh vertices
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> vertCount;

  // Load in the number of mesh indices
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> indexCount;

  // Load in the number of mesh normals
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> normalCount;

  // Load in the number of voxels
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> voxelCount;

  glm::uvec3 extentsVoxelSpace(0, 0, 0);

  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> extentsVoxelSpace.x >> extentsVoxelSpace.y >> extentsVoxelSpace.z;

  glm::vec3 extentsObjectSpace(0.0f);
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> extentsObjectSpace.x >> extentsObjectSpace.y >> extentsObjectSpace.z;

  glm::vec3 initialVoxelSize(0.0f);
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> initialVoxelSize.x >> initialVoxelSize.y >> initialVoxelSize.z;

  Mesh *mesh = new Mesh;
  mesh->mVertices.SetBufferSize(vertCount);
  mesh->mIndices.resize(indexCount);
  mesh->mNormals.SetBufferSize(normalCount);

  auto vm = new VoxObj::VoxelMesh(extentsVoxelSpace, extentsObjectSpace, initialVoxelSize, mesh);

  // Load in the mesh vertices
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (u64 i = 0; i < vertCount; i++)
  {
    file >> mesh->mVertices.AccessBuffer(i);
  }
  // Load in the mesh indices
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (u64 i = 0; i < indexCount; i++)
  {
    file >> mesh->mIndices[i];
  }
  // Load in the mesh normals
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (u64 i = 0; i < normalCount; i++)
  {
    file >> mesh->mNormals.AccessBuffer(i);
  }
  // Start loading in the voxels
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (u64 i = 0; i < voxelCount; i++)
  {
    VoxObj::Voxel voxel;
    glm::uvec3 key;
    // Load the voxel key (also its position in voxel space)
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file >> key.x >> key.y >> key.z;

    // Load in the voxel dimensions
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file >> voxel.mDimensions.x >> voxel.mDimensions.y >> voxel.mDimensions.z;

    // Load in the voxel position
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file >> voxel.mPosition.x >> voxel.mPosition.y >> voxel.mPosition.z;

    // Load in the number of neighbors this voxel has
    u64 neighborCount = 0;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file >> neighborCount;

    // Load all the voxel neighbor keys
    voxel.mNeighbors.resize(neighborCount);
    for (u64 n = 0; n < neighborCount; n++)
    {
      file >> voxel.mNeighbors[n].x >> voxel.mNeighbors[n].y >> voxel.mNeighbors[n].z;
    }

    // Load in the number of contained vertices
    u64 containedVertCount = 0;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file >> containedVertCount;

    // Load in the contained vertices
    voxel.mMeshVertices.resize(containedVertCount);
    for (u64 v = 0; v < containedVertCount; v++)
    {
      file >> voxel.mMeshVertices[v];
    }

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    u64 bezierCount = 0;
    file >> bezierCount;
    voxel.mBezierCurves.resize(bezierCount);

    for (u64 b = 0; b < bezierCount; b++)
    {
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      file >> voxel.mBezierCurves[b].mFirstT;
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      file >> voxel.mBezierCurves[b].mSecondT;
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      u64 controlPointCount = 0;
      file >> controlPointCount;
      voxel.mBezierCurves[b].mControlPoints.resize(controlPointCount);
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      for (u64 cp = 0; cp < controlPointCount; cp++)
      {
        file >> voxel.mBezierCurves[b].mControlPoints[cp].x
            >> voxel.mBezierCurves[b].mControlPoints[cp].y
            >> voxel.mBezierCurves[b].mControlPoints[cp].z;
      }

      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      u64 effectPointCount = 0;
      file >> effectPointCount;
      voxel.mBezierCurves[b].mEffectedPoints.resize(effectPointCount);
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      for (u64 effectedPoint = 0; effectedPoint < effectPointCount; effectedPoint++)
      {
        file >> voxel.mBezierCurves[b].mEffectedPoints[effectedPoint];
      }
    }

    vm->SetVoxel(key, voxel);
  }
  return vm;
}
} // namespace Utils
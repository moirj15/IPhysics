#include "obj.h"

#include "../renderer/mesh.h"

#include <cstdio>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/normal.hpp>
#include <sstream>

namespace VoxGen
{
ObjReader::ObjReader() : mPresentBitSet(0), mDataLen(0), mPos(0), mMesh(new Mesh)
{
}

Mesh *ObjReader::Parse(const char *filename)
{
  LoadOBJFile(filename);
  while (mPos < mDataLen)
  {
    auto dataType = ParseType();
    switch (dataType)
    {
    case DataType::Vertex:
      ParseVertex();
      break;
    case DataType::TextureCoord:
      fprintf(stderr, "NOT IMPLEMENTED\n");
      break;
    case DataType::VertexNormal:
      ParseNormal();
      break;
    case DataType::ParameterSpaceVertex:
      fprintf(stderr, "NOT IMPLEMENTED\n");
      break;
    case DataType::PolygonFace:
      ParseFace();
      break;
    case DataType::LineElement:
      fprintf(stderr, "NOT IMPLEMENTED\n");
      break;
    case DataType::Object:
      fprintf(stderr, "NOT IMPLEMENTED\n");
      SkipLine();
      break;
    case DataType::Group:
      fprintf(stderr, "NOT IMPLEMENTED\n");
      break;
    case DataType::Comment:
      SkipLine();
      break;
    case DataType::Unknown:
      fprintf(stderr, "NOT IMPLEMENTED\n");
      SkipLine();
      break;
    }
  }
  //   mMesh->mNormals.SetBufferSize(mTempNormals.size() * 3);
  //   mMesh->mNormals.resize(mMesh->mIndecies.size() * 3);
  //   for (u32 i = 0; i < mMesh->mIndecies.size(); i += 3)
  //   {
  //     u32 i0 = mMesh->mIndecies[i];
  //     u32 i1 = mMesh->mIndecies[i + 1];
  //     u32 i2 = mMesh->mIndecies[i + 2];
  //     glm::vec3 t0(mMesh->mVertecies[i0], mMesh->mVertecies[i0 + 1], mMesh->mVertecies[i0 + 2]);
  //     glm::vec3 t1(mMesh->mVertecies[i1], mMesh->mVertecies[i1 + 1], mMesh->mVertecies[i1 + 2]);
  //     glm::vec3 t2(mMesh->mVertecies[i2], mMesh->mVertecies[i2 + 1], mMesh->mVertecies[i2 + 2]);
  //     auto normal = glm::triangleNormal(t0, t1, t2);
  //     mMesh->mNormals[i0] += normal.x;
  //     mMesh->mNormals[i0 + 1] += normal.y;
  //     mMesh->mNormals[i0 + 2] += normal.z;
  //
  //     mMesh->mNormals[i1] += normal.x;
  //     mMesh->mNormals[i1 + 1] += normal.y;
  //     mMesh->mNormals[i1 + 2] += normal.z;
  //
  //     mMesh->mNormals[i2] += normal.x;
  //     mMesh->mNormals[i2 + 1] += normal.y;
  //     mMesh->mNormals[i2 + 2] += normal.z;
  //   }
  //   for (u32 i = 0; i < mMesh->mNormals.size(); i++)
  //   {
  //     mMesh->mNormals[i] /= 3.0f;
  //   }
  Clear();
  // This should clear out mMesh
  return mMesh;
}

void ObjReader::Clear()
{
  mPresentBitSet = 0;
  mTempNormals.clear();
  mData.reset();
  mDataLen = 0;
  mPos = 0;
  mFilename.clear();
}

void ObjReader::LoadOBJFile(const char *filename)
{
  mFilename = filename;
  FILE *fp = OpenFile(filename, "rb");
  char *data = nullptr;
  Size len = 0;

  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  rewind(fp);

  if (len == 0)
  {
    fprintf(stderr, "failed to get file size");
  }

  data = new char[len + 1]();
  mDataLen = len + 1;
  len = fread(data, sizeof(char), len, fp);
  mData.reset(data);

  fclose(fp);
}

// Private
ObjReader::DataType ObjReader::ParseType()
{
  if (Token() == 'f')
  {
    mPos += 2;
    return DataType::PolygonFace;
  }
  else if (Token() == 'l')
  {
    mPos += 2;
    return DataType::LineElement;
  }
  else if (Token() == 'o')
  {
    mPos += 2;
    return DataType::Object;
  }
  else if (Token() == 'v')
  {
    mPos++;
    if (Token() == ' ')
    {
      mPos++;
      mPresentBitSet |= (u32)Present::Vertex;
      return DataType::Vertex;
    }
    else if (Token() == 't')
    {
      mPos += 2;
      mPresentBitSet |= (u32)Present::Texture;
      return DataType::TextureCoord;
    }
    else if (Token() == 'p')
    {
      mPos += 2;
      return DataType::ParameterSpaceVertex;
    }
    else if (Token() == 'n')
    {
      mPos += 2;
      mPresentBitSet |= (u32)Present::Normal;
      return DataType::VertexNormal;
    }
  }
  else if (Token() == '#')
  {
    return DataType::Comment;
  }
  fprintf(stderr, "Unrecognized OBJ datatype\n");
  return DataType::Unknown;
}

void ObjReader::ParseVertex()
{
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  for (s32 i = 0; i < 3; i++)
  {
    line >> val;
    mMesh->mVertices.BufferPushBack(val);
  }
}

void ObjReader::ParseNormal()
{
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  glm::vec3 normal;
  for (s32 i = 0; i < 3; i++)
  {
    line >> normal[i];
  }
  mTempNormals.emplace_back(normal);
}

void ObjReader::ParseFace()
{
  //   if (mMesh->mNormals.IsEmpty())
  //   {
  //     mMesh->mNormals.SetBufferSize(mTempNormals.size() * 3);
  //   }
  std::string lineStr{ReadLine()};
  //   ReplaceChars(&lineStr, '/', ' ');
  std::stringstream line{lineStr};
  //   u32 index = 0;
  //   u32 texCoord = 0;
  //   u32 normal = 0;
  for (s32 i = 0; i < 3; i++)
  {
    // NOTE: only indices are read in
    u32 index = 0.0;
    u32 normal = 0.0;
    line >> index; // >> normal;
    index--;
    //     normal--;
    mMesh->mIndices.emplace_back(index);
    //     mMesh->mNormals.AccessCastBuffer(index) = mTempNormals[normal];
    //     mMesh->mNormals.AccessBuffer((index * 3)) = mTempNormals[normal].x;
    //     mMesh->mNormals.AccessBuffer((index * 3) + 1) = mTempNormals[normal].y;
    //     mMesh->mNormals.AccessBuffer((index * 3) + 2) = mTempNormals[normal].z;
  }
}

std::string ObjReader::ReadLine()
{
  std::string line = "";
  while (Token() != '\n' && mPos < mDataLen)
  {
    line += Token();
    mPos++;
  }
  mPos++;
  return line;
}

void ObjReader::SkipLine()
{
  while (Token() != '\n' && mPos < mDataLen)
  {
    mPos++;
  }
  mPos++;
}

void ObjReader::ReplaceChars(std::string *str, char toReplace, char replacement)
{
  for (Size i = 0; i < str->length(); i++)
  {
    if ((*str)[i] == toReplace)
    {
      (*str)[i] = replacement;
    }
  }
}

} // namespace VoxGen

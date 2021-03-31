#include "Obj.h"

#include <Renderer/Mesh.h>
#include <cstdio>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/normal.hpp>
#include <sstream>

ObjReader::ObjReader() : mPresentBitSet(0), mDataLen(0), mPos(0), mMesh(new objs::Mesh)
{
}

objs::Mesh *ObjReader::Parse(const char *filename)
{
  LoadOBJFile(filename);
  while (mPos < mDataLen) {
    auto dataType = ParseType();
    switch (dataType) {
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
  mMesh->normals.resize(mMesh->indices.size() * 3);
  // TODO: actually calculate surface normals or just use blender's normals
  for (u32 i : mMesh->indices) {
    auto v = mMesh->GetVertex(i);
    auto n = glm::normalize(v);
    mMesh->SetNormal(i, n);
  }

//  for (u32 i = 0; i < mMesh->indices.size(); i += 3) {
//    u32 i0 = mMesh->indices[i];
//    u32 i1 = mMesh->indices[i + 1];
//    u32 i2 = mMesh->indices[i + 2];
//    auto t0 = mMesh->GetVertex(i0);
//    auto t1 = mMesh->GetVertex(i1);
//    auto t2 = mMesh->GetVertex(i2);
//    auto normal = glm::triangleNormal(t0, t1, t2);
//    mMesh->normals[i0] += normal.x;
//    mMesh->normals[i0 + 1] += normal.y;
//    mMesh->normals[i0 + 2] += normal.z;
//
//    mMesh->normals[i1] += normal.x;
//    mMesh->normals[i1 + 1] += normal.y;
//    mMesh->normals[i1 + 2] += normal.z;
//
//    mMesh->normals[i2] += normal.x;
//    mMesh->normals[i2 + 1] += normal.y;
//    mMesh->normals[i2 + 2] += normal.z;
//  }
//  for (u32 i = 0; i < mMesh->normals.size(); i++) {
//    mMesh->normals[i] /= 3.0f;
//  }
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

  if (len == 0) {
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
  if (Token() == 'f') {
    mPos += 2;
    return DataType::PolygonFace;
  } else if (Token() == 'l') {
    mPos += 2;
    return DataType::LineElement;
  } else if (Token() == 'o') {
    mPos += 2;
    return DataType::Object;
  } else if (Token() == 'v') {
    mPos++;
    if (Token() == ' ') {
      mPos++;
      mPresentBitSet |= (u32)Present::Vertex;
      return DataType::Vertex;
    } else if (Token() == 't') {
      mPos += 2;
      mPresentBitSet |= (u32)Present::Texture;
      return DataType::TextureCoord;
    } else if (Token() == 'p') {
      mPos += 2;
      return DataType::ParameterSpaceVertex;
    } else if (Token() == 'n') {
      mPos += 2;
      mPresentBitSet |= (u32)Present::Normal;
      return DataType::VertexNormal;
    }
  } else if (Token() == '#') {
    return DataType::Comment;
  }
  fprintf(stderr, "Unrecognized OBJ datatype\n");
  return DataType::Unknown;
}

void ObjReader::ParseVertex()
{
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  for (s32 i = 0; i < 3; i++) {
    line >> val;
    mMesh->vertices.push_back(val);
  }
}

void ObjReader::ParseNormal()
{
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  glm::vec3 normal;
  for (s32 i = 0; i < 3; i++) {
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
  ReplaceChars(&lineStr, '/', ' ');
  std::stringstream line{lineStr};
  //   u32 index = 0;
  //   u32 texCoord = 0;
  //   u32 normal = 0;
  for (s32 i = 0; i < 3; i++) {
    // NOTE: only indices are read in
    u32 index = 0.0;
    // u32 normal = 0.0;
    line >> index; // >> normal;
    index--;
    //     normal--;
    mMesh->indices.emplace_back(index);
    //     mMesh->mNormals.AccessCastBuffer(index) = mTempNormals[normal];
    //     mMesh->mNormals.AccessBuffer((index * 3)) = mTempNormals[normal].x;
    //     mMesh->mNormals.AccessBuffer((index * 3) + 1) = mTempNormals[normal].y;
    //     mMesh->mNormals.AccessBuffer((index * 3) + 2) = mTempNormals[normal].z;
  }
}

std::string ObjReader::ReadLine()
{
  std::string line = "";
  while (Token() != '\n' && mPos < mDataLen) {
    line += Token();
    mPos++;
  }
  mPos++;
  return line;
}

void ObjReader::SkipLine()
{
  while (Token() != '\n' && mPos < mDataLen) {
    mPos++;
  }
  mPos++;
}

void ObjReader::ReplaceChars(std::string *str, char toReplace, char replacement)
{
  for (Size i = 0; i < str->length(); i++) {
    if ((*str)[i] == toReplace) {
      (*str)[i] = replacement;
    }
  }
}

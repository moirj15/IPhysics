#include "Obj.h"

#include <Renderer/Mesh.h>
#include <cstdio>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/normal.hpp>
#include <sstream>

ObjParser::ObjParser() : mPresentBitSet(0), mDataLen(0), mPos(0), mMesh(new objs::Mesh)
{
}

objs::Mesh *ObjParser::Parse(const char *filename)
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

  Clear();
  // This should clear out mMesh
  return mMesh;
}

void ObjParser::Clear()
{
  mPresentBitSet = 0;
  mTempNormals.clear();
  mData.reset();
  mDataLen = 0;
  mPos = 0;
  mFilename.clear();
}

void ObjParser::LoadOBJFile(const char *filename)
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
ObjParser::DataType ObjParser::ParseType()
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

void ObjParser::ParseVertex()
{
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  for (s32 i = 0; i < 3; i++) {
    line >> val;
    mMesh->vertices.push_back(val);
  }
}

void ObjParser::ParseNormal()
{
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  glm::vec3 normal;
  for (s32 i = 0; i < 3; i++) {
    line >> normal[i];
  }
  mTempNormals.emplace_back(normal);
}

void ObjParser::ParseFace()
{
  std::string lineStr{ReadLine()};
  ReplaceChars(&lineStr, '/', ' ');
  std::stringstream line{lineStr};
  for (s32 i = 0; i < 3; i++) {
    u32 index = 0;
    u32 texCoord = 0; // unused for now
    u32 normal = 0;
    if (mPresentBitSet & (u32)Present::Texture) {
      line >> index >> texCoord >> normal;
    } else {
      line >> index >> normal;
    }
    index--;
    normal--;
    mMesh->indices.emplace_back(index);
  }
}

std::string ObjParser::ReadLine()
{
  std::string line = "";
  while (Token() != '\n' && mPos < mDataLen) {
    line += Token();
    mPos++;
  }
  mPos++;
  return line;
}

void ObjParser::SkipLine()
{
  while (Token() != '\n' && mPos < mDataLen) {
    mPos++;
  }
  mPos++;
}

void ObjParser::ReplaceChars(std::string *str, char toReplace, char replacement)
{
  for (Size i = 0; i < str->length(); i++) {
    if ((*str)[i] == toReplace) {
      (*str)[i] = replacement;
    }
  }
}

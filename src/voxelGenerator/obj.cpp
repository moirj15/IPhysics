#include "obj.h"

#include "../renderer/mesh.h"

#include <cstdio>
#include <sstream>

namespace vg
{
ObjReader::ObjReader() : mFilename(), mDataLen(0), mPos(0), mMesh(new Mesh)
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
  Clear();
  // This should clear out mMesh
  return mMesh;
}

void ObjReader::Clear()
{
  mData.reset();
  mDataLen = 0;
  mPos = 0;
  mFilename.clear();
}

void ObjReader::LoadOBJFile(const char *filename)
{
  mFilename = filename;
  FILE *fp = OpenFile(filename, "r");
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
      return DataType::Vertex;
    }
    else if (Token() == 't')
    {
      mPos += 2;
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
    mMesh->mVertecies.emplace_back(val);
  }
}

void ObjReader::ParseNormal()
{
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  for (s32 i = 0; i < 3; i++)
  {
    line >> val;
    mMesh->mNormals.emplace_back(val);
  }
}

void ObjReader::ParseFace()
{
  std::string lineStr{ReadLine()};
  ReplaceChars(&lineStr, '/', ' ');
  std::stringstream line{lineStr};
  for (s32 i = 0; i < 3; i++)
  {
    u32 val = 0.0f;
    u32 junk = 0.0f;
    line >> val >> junk;
    mMesh->mIndecies.emplace_back(val - 1);
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

} // namespace vg

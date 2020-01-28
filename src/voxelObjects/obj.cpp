#include "obj.h"

#include "renderer/VertexBuffer.h"

#include <cstdio>
#include <sstream>

ObjReader::ObjReader(char *filename) : m_filename(std::move(filename)), m_dataLen(0), m_pos(0) {
  FILE *fp = OpenFile(m_filename.c_str(), "r");
  char *data = nullptr;
  Size len = 0;

  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  rewind(fp);

  if (len == 0) {
    fprintf(stderr, "failed to get file size");
  }

  // m_data.reserve(len + 1);
  data = new char[len + 1]();
  m_dataLen = len + 1;
  len = fread(data, sizeof(char), len, fp);
  // len = fread(&m_data[0], sizeof(char), len, fp);
  m_data.reset(data);

  fclose(fp);
}

Mesh *ObjReader::Parse() {
  while (m_pos < m_dataLen) {
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
  // This should clear out m_mesh
  return new Mesh{std::move(m_mesh)};
}

void ObjReader::Clear() {
  m_data.reset();
  m_dataLen = 0;
  m_pos = 0;
  m_filename.clear();
}

// Private
ObjReader::DataType ObjReader::ParseType() {
  if (Token() == 'f') {
    m_pos += 2;
    return DataType::PolygonFace;
  } else if (Token() == 'l') {
    m_pos += 2;
    return DataType::LineElement;
  } else if (Token() == 'o') {
    m_pos += 2;
    return DataType::Object;
  } else if (Token() == 'v') {
    m_pos++;
    if (Token() == ' ') {
      m_pos++;
      return DataType::Vertex;
    } else if (Token() == 't') {
      m_pos += 2;
      return DataType::TextureCoord;
    } else if (Token() == 'p') {
      m_pos += 2;
      return DataType::ParameterSpaceVertex;
    } else if (Token() == 'n') {
      m_pos += 2;
      return DataType::VertexNormal;
    }
  } else if (Token() == '#') {
    return DataType::Comment;
  }
  fprintf(stderr, "Unrecognized OBJ datatype\n");
  return DataType::Unknown;
}

void ObjReader::ParseVertex() {
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  for (s32 i = 0; i < 3; i++) {
    line >> val;
    m_mesh.vertecies.emplace_back(val);
  }
}

void ObjReader::ParseNormal() {
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  for (s32 i = 0; i < 3; i++) {
    line >> val;
    m_mesh.normals.emplace_back(val);
  }
}

void ObjReader::ParseFace() {
  std::string lineStr{ReadLine()};
  ReplaceChars(&lineStr, '/', ' ');
  std::stringstream line{lineStr};
  for (s32 i = 0; i < 3; i++) {
    u32 val = 0.0f;
    u32 junk = 0.0f;
    line >> val >> junk;
    m_mesh.connections.emplace_back(val - 1);
  }
}

std::string ObjReader::ReadLine() {
  std::string line = "";
  while (Token() != '\n' && m_pos < m_dataLen) {
    line += Token();
    m_pos++;
  }
  m_pos++;
  return line;
}

void ObjReader::SkipLine() {
  while (Token() != '\n' && m_pos < m_dataLen) {
    m_pos++;
  }
  m_pos++;
}

void ObjReader::ReplaceChars(std::string *str, char toReplace, char replacement) {
  for (Size i = 0; i < str->length(); i++) {
    if ((*str)[i] == toReplace) {
      (*str)[i] = replacement;
    }
  }
}

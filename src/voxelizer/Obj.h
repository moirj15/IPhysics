#pragma once

#include <Common.h>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>
#include "Objects.h"

//struct Mesh;


// Will only work with obj files that contain vertices and indices. No normals or uv coordinates
class ObjReader
{
  enum class Present : u32
  {
    Vertex = 1 << 0,
    Texture = 1 << 1,
    Normal = 1 << 2,
  };
  u32 mPresentBitSet;
  std::string mFilename;
  std::unique_ptr<char[]> mData;
  std::vector<glm::vec3> mTempNormals;
  Size mDataLen;

  Size mPos;

  objs::Mesh *mMesh;

  enum class DataType
  {
    Vertex,
    TextureCoord,
    VertexNormal,
    ParameterSpaceVertex,
    PolygonFace,
    LineElement,
    Object,
    Group,
    Comment,
    Unknown
  };

public:
  /**
   * Constructor.
   * @param filename: The obj filename that will be parsed.
   */
  ObjReader();
  /// Parses the obj file and returns a mesh
  objs::Mesh *Parse(const char *filename);

  /// Clears the parser.
  void Clear();

private:
  void LoadOBJFile(const char *filename);

  /// Determines the parse type of the current token.
  DataType ParseType();

  /// Getter for the current token.
  inline char Token()
  {
    return mData[mPos];
  }

  /// Parses a vertex from the file.
  void ParseVertex();

  /// Parses a normal from the file.
  void ParseNormal();

  /// parses a face from thefile.
  void ParseFace();

  /// Reads a line from the file.
  std::string ReadLine();

  /// Skips a line of the file.
  void SkipLine();

  /**
   * Replaces the specifide char with the given replacement.
   * @param str: The string that will have a character replaced.
   * @param toReplace: THe character that will be replaced.
   * @param replacement: The replacement character.
   */
  void ReplaceChars(std::string *str, char toReplace, char replacement);
};


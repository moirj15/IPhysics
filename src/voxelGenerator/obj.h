#pragma once

#include "../common.h"

#include <memory>
#include <string>
#include <vector>

struct Mesh;

namespace VoxGen
{

// struct Mesh
// {
//   std::vector<f32> vertecies = {};
//   std::vector<u32> connections = {};
//   std::vector<f32> normals = {};
//   u32 vertexSize = 3;
//
//   Mesh() = default;
//   Mesh(Mesh &&m) :
//       vertecies(std::move(m.vertecies)), connections(std::move(m.connections)),
//       normals(std::move(m.normals)), vertexSize(m.vertexSize)
//   {
//   }
// };

class ObjReader
{
  std::string mFilename;
  std::unique_ptr<char[]> mData;
  Size mDataLen;

  Size mPos;

  Mesh *mMesh;

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
  Mesh *Parse(const char *filename);

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

} // namespace vg

#ifndef OBJ_H
#define OBJ_H

#include "common.h"

#include <memory>
#include <string>
#include <vector>

struct Mesh {
  std::vector<f32> vertecies = {};
  std::vector<u32> connections = {};
  std::vector<f32> normals = {};
  u32 vertexSize = 3;

  Mesh() = default;
  Mesh(Mesh &&m) :
      vertecies(std::move(m.vertecies)), connections(std::move(m.connections)),
      normals(std::move(m.normals)), vertexSize(m.vertexSize) {}
};

class ObjReader {
  std::string m_filename;
  // std::vector<char> m_data;
  std::unique_ptr<char[]> m_data;
  Size m_dataLen;

  Size m_pos;

  Mesh m_mesh;

  enum class DataType {
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
  ObjReader(char *filename);

  /// Parses the obj file and returns a mesh
  Mesh *Parse();

  /// Clears the parser.
  void Clear();

private:
  /// Determines the parse type of the current token.
  DataType ParseType();

  /// Getter for the current token.
  inline char Token() { return m_data[m_pos]; }

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

#endif // OBJ_H

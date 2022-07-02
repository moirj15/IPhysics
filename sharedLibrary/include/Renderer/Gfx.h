#pragma once

#include <Common.h>
struct SDL_Window;
namespace gfx
{
const u32 INVALID_HANDLE = 0;

template<typename Tag, typename T, T default_value>
class Handle
{
  T _value;

public:
  static Handle Invalid() { return Handle(default_value); }

  Handle() : _value(default_value) {}
  explicit Handle(T value) : _value(value) {}

  T Get() const { return _value; }

  friend bool operator==(Handle a, Handle b) { return a._value == b._value; }
  friend bool operator!=(Handle a, Handle b) { return a._value != b._value; }
  bool operator==(T b) { return _value == b; }
  bool operator!=(T b) { return _value != b; }
  // T operator++(int) { return _value++; }
  // size_t Hash() const { return std::hash<T>()(_value); }
};

struct VertexBufferHandleTag {
};
struct IndexBufferHandleTag {
};
struct ShaderHandleTag {
};

using VertexBufferHandle = Handle<VertexBufferHandleTag, u32, INVALID_HANDLE>;
using IndexBufferHandle = Handle<VertexBufferHandleTag, u32, INVALID_HANDLE>;
using ShaderHandle = Handle<ShaderHandleTag, u32, INVALID_HANDLE>;

enum class BufferUsage {
  StreamDraw,
  StreamRead,
  StreamCopy,
  StaticDraw,
  StaticRead,
  StaticCopy,
  DynamicDraw,
  DynamicRead,
  DynamicCopy
};

enum class VarType {
  Int,
  Int2,
  Int3,
  Int4,
  Int2x2,
  Int3x3,
  Int4x4,
  Float,
  Float2,
  Float3,
  Float4,
  Float2x2,
  Float3x3,
  Float4x4,
};

enum class IndexType {
  U8,
  U16,
  U32,
};

struct Window {
  SDL_Window *sdl_window;
  u32 width;
  u32 height;
};

enum class ShaderType {
  Vertex,
  Fragment,
};

enum class Primitive {
  Points,
  LineStrip,
  LineLoop,
  Lines,
  LineStripAdjacency,
  LinesAdjacency,
  TriangleStrip,
  TriangleFan,
  Triangles,
  TriangleStripAdjacency,
};

void Init(const Window &window);

Window CreateWindow(u32 width, u32 height);
void DeleteWindow(Window &window);

VertexBufferHandle CreateVertexBuffer(const u64 size_in_bytes, const void *data, const BufferUsage usage,
    const VarType *contained_types, const u64 contained_types_count);
void DeleteVertexBuffer(VertexBufferHandle &handle);

IndexBufferHandle CreateIndexBuffer(const u64 size_in_bytes, const void *data, const BufferUsage usage, const IndexType index_type);
void DeleteIndexBuffer(IndexBufferHandle &handle);

ShaderHandle CreateShader(const char *source, const u64 length, const ShaderType type);
//void ShaderAddVertexAttribute()
void DeleteShader(ShaderHandle &handle);

void Draw(const VertexBufferHandle vertex_buffer, const ShaderHandle shader_handle, const u64 triangles);
} // namespace gfx

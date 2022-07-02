#include <Renderer/Gfx.h>
#include <unordered_map>
#include <vector>
#include <Renderer/glad.h>
#include <SDL2/SDL.h>
#include <cassert>

#if 0
namespace gfx
{
static std::unordered_map<GLuint, std::vector<VarType>> _vertex_buffer_types;
static std::unordered_map<GLuint, IndexType> _index_buffer_types;

static GLenum GetGLUsage(const BufferUsage usage)
{
  switch (usage) {
  case BufferUsage::StreamDraw:
    return GL_STREAM_DRAW;
  case BufferUsage::StreamRead:
    return GL_STREAM_READ;
  case BufferUsage::StreamCopy:
    return GL_STREAM_COPY;
  case BufferUsage::StaticDraw:
    return GL_STATIC_DRAW;
  case BufferUsage::StaticRead:
    return GL_STATIC_READ;
  case BufferUsage::StaticCopy:
    return GL_STATIC_COPY;
  case BufferUsage::DynamicDraw:
    return GL_DYNAMIC_DRAW;
  case BufferUsage::DynamicRead:
    return GL_DYNAMIC_READ;
  case BufferUsage::DynamicCopy:
    return GL_DYNAMIC_COPY;
  }
}

static GLenum GetGLShaderType(const ShaderType type)
{
  switch (type) {
  case ShaderType::Vertex:
    return GL_VERTEX_SHADER;
  case ShaderType::Fragment:
    return GL_FRAGMENT_SHADER;
  }
}

static GLbitfield GetGLShaderBit(const ShaderType type)
{
  switch (type) {
  case ShaderType::Vertex:
    return GL_VERTEX_SHADER_BIT;
  case ShaderType::Fragment:
    return GL_FRAGMENT_SHADER_BIT;
  }
}

static GLuint DEFAULT_VAO;

void Init(const Window &window)
{
  SDL_GL_LoadLibrary(nullptr);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  auto *context = SDL_GL_CreateContext(window.sdl_window);
  assert(context);
  assert(gladLoadGL());

  glGenVertexArrays(1, &DEFAULT_VAO);
}

Window CreateWindow(u32 width, u32 height)
{
}
void DeleteWindow(Window &window)
{
}

static GLuint CreateBuffer(const u64 size_in_bytes, const void *data, const GLenum buffer_usage)
{
  GLuint gl_handle = 0;
  glCreateBuffers(1, &gl_handle);
  glNamedBufferData(gl_handle, size_in_bytes, data, buffer_usage);
  return gl_handle;
}

template<typename Handle>
static void DeleteBuffer(Handle &handle)
{
  GLuint h = handle.Get();
  glDeleteBuffers(1, &h);
  handle = Handle(Handle::Invalid());
}

VertexBufferHandle CreateVertexBuffer(const u64 size_in_bytes, const void *data, const BufferUsage usage,
    const VarType *contained_types, const u64 contained_types_count)
{
  auto gl_handle = CreateBuffer(size_in_bytes, data, GetGLUsage(usage));
  std::vector<VarType> types;
  types.assign(contained_types, contained_types + contained_types_count);
  _vertex_buffer_types.insert({gl_handle, types});
  return VertexBufferHandle(gl_handle);
}
void DeleteVertexBuffer(VertexBufferHandle &handle)
{
  DeleteBuffer(handle);
}

IndexBufferHandle CreateIndexBuffer(
    const u64 size_in_bytes, const void *data, const BufferUsage usage, const IndexType index_type)
{
  auto gl_handle = CreateBuffer(size_in_bytes, data, GetGLUsage(usage));
  _index_buffer_types.insert({gl_handle, index_type});
  return IndexBufferHandle(gl_handle);
}
void DeleteIndexBuffer(IndexBufferHandle &handle)
{
  DeleteBuffer(handle);
}

ShaderHandle CreateShader(const char *source, const u64 length, const ShaderType type)
{
  
}
void DeleteShader(ShaderHandle &handle)
{
}

void Draw(const VertexBufferHandle vertex_buffer, const ShaderHandle shader_handle, const u64 triangles)
{
}

} // namespace gfx
#endif

#include <Renderer/RendererFrontend.h>

//#include "../../PhysicsEngine/Settings.h"
//#include "../../voxelObjects/VoxelMesh.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <Renderer/Mesh.h>
#include <Renderer/RendererBackend.h>
#include <Renderer/Shader.h>
#include <Renderer/Window.h>
#include <unordered_map>
namespace Renderer
{

constexpr u32 MAT4_SIZE = 16;
constexpr u32 VEC3_SIZE = 3;
constexpr u32 VEC2_SIZE = 2;

static GLFWwindow *sWindow;

static MeshHandle sCurrentMeshHandle;

static std::unordered_map<ShaderProgram, Shader> sShaders;
static std::unordered_map<ShaderProgram, std::vector<std::string>> sShaderPaths = {
    {ShaderProgram::UniformColor, {"../shaders/uniformColor.vert", "../shaders/uniformColor.frag"}},
    {ShaderProgram::FlatLight, {"../shaders/flatLight.vert", "../shaders/flatLight.frag"}},
    {ShaderProgram::PhongLight, {"../shaders/phongLight.vert", "../shaders/phongLight.frag"}},
    {ShaderProgram::Point, {"../shaders/point.vert", "../shaders/point.frag"}},
};

const char *positionAttribute = "vPosition";
const char *normalAttribute = "vNormal";
const char *offsetAttribute = "vOffset";
const char *textureAttribute = "vTextureCoordinate";

struct IndexBuffer
{
  u32 size;
  u32 handle;
};

struct BufferLayout
{
  std::string mName;
  u32 mElementSize;
  u32 mElementStride;
  s32 mLocation;
  GLenum mType;
};

struct VertexBuffer
{
  std::vector<BufferLayout> layouts;
  u32 handle;
};

static inline u32 CreateBuffer(GLenum bufferType, void *data, u32 len, GLenum usage)
{
  u32 handle = 0;
  glGenBuffers(1, &handle);
  glBindBuffer(bufferType, handle);
  glBufferData(bufferType, len, data, usage);
  return handle;
}

struct VertexArray
{
  IndexBuffer indexBuffer;
  std::unordered_map<s32, VertexBuffer> vertexBuffers;
  u32 handle;
  ShaderProgram program;

  void AddVertexBuffer(
      s32 location, const char *attributeName, u32 attribSize, u32 stride, GLenum attribType,
      GLenum bufferType, void *data, u32 dataSize, GLenum usage)
  {
    BufferLayout layout = {positionAttribute, VEC3_SIZE, 0, location, GL_FLOAT};
    VertexBuffer vb = {{layout}, CreateBuffer(bufferType, data, dataSize, usage)};
    vertexBuffers.insert(std::make_pair(location, vb));
  }

  void Destroy()
  {
    for (auto &vb : vertexBuffers)
    {
      glDeleteBuffers(1, &vb.second.handle);
    }
    glDeleteBuffers(1, &indexBuffer.handle);
    glDeleteVertexArrays(1, &handle);
  }
};

static std::unordered_map<MeshHandle, VertexArray> sVertexArrays;

static void ErrorCallback(
    GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char *message,
    const void *userParam)
{
  printf("%s\n", message);
}

Window *Init(s32 width, s32 height, const char *windowName, bool enableDebug)
{
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to init GLFW\n");
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  Window *window = new Window(width, height, windowName);
  sWindow = window->mGLWindow;

  glfwMakeContextCurrent(window->mGLWindow);
  if (!gladLoadGL())
  {
    fprintf(stderr, "Failed to load glad\n");
  }
  glfwSwapInterval(1);
  glEnable(GL_DEPTH_TEST);
  glLineWidth(3.0f);
  // glfwSwapInterval(0);
  //  glCullFace(GL_BACK);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glClearDepth(4.0);
  glDepthFunc(GL_LESS);

  glDebugMessageCallback(ErrorCallback, nullptr);
  if (enableDebug)
  {
    glEnable(GL_DEBUG_OUTPUT);
  }
  for (const auto &[key, paths] : sShaderPaths)
  {
    sShaders.emplace(key, Shader(paths));
  }
  return window;
}

static MeshHandle StoreMesh(Mesh *mesh, GLenum type, ShaderProgram program, GLenum usage)
{
  VertexArray vertexArray;
  glGenVertexArrays(1, &vertexArray.handle);
  glBindVertexArray(vertexArray.handle);
  vertexArray.program = program;
  // for now just throw each of these into their own attribute slot
  if (!mesh->mVertices.IsEmpty())
  {
    s32 location = sShaders[program].getAttributeLocation(positionAttribute);
    vertexArray.AddVertexBuffer(
        location, positionAttribute, VEC3_SIZE, 0, GL_FLOAT, type,
        (void *)mesh->mVertices.BufferPtr(), mesh->mVertices.BufferSizeBytes(), usage);
  }
  if (!mesh->mNormals.IsEmpty())
  {
    s32 location = sShaders[program].getAttributeLocation(normalAttribute);
    vertexArray.AddVertexBuffer(
        location, normalAttribute, VEC3_SIZE, 0, GL_FLOAT, type, (void *)mesh->mNormals.BufferPtr(),
        mesh->mNormals.BufferSizeBytes(), usage);
  }
  if (!mesh->mOffsets.IsEmpty())
  {
    s32 location = sShaders[program].getAttributeLocation(offsetAttribute);
    vertexArray.AddVertexBuffer(
        location, offsetAttribute, VEC3_SIZE, 0, GL_FLOAT, type, (void *)mesh->mOffsets.BufferPtr(),
        mesh->mOffsets.BufferSizeBytes(), usage);
  }
  if (!mesh->mTextureCoords.IsEmpty())
  {
    s32 location = sShaders[program].getAttributeLocation(textureAttribute);
    vertexArray.AddVertexBuffer(
        location, textureAttribute, VEC2_SIZE, 0, GL_FLOAT, type,
        (void *)mesh->mTextureCoords.BufferPtr(), mesh->mTextureCoords.BufferSizeBytes(), usage);
  }

  vertexArray.indexBuffer = {
      (u32)mesh->mIndices.size(), CreateBuffer(
                                      GL_ELEMENT_ARRAY_BUFFER, (void *)mesh->mIndices.data(),
                                      mesh->mIndices.size() * sizeof(u32), usage)};
  glBindVertexArray(0);

  sCurrentMeshHandle++;
  sVertexArrays.emplace(sCurrentMeshHandle, vertexArray);
  return sCurrentMeshHandle;
}

NODISCARD MeshHandle SubmitDynamicMesh(Mesh *mesh, ShaderProgram program)
{
  return StoreMesh(mesh, GL_ARRAY_BUFFER, program, GL_DYNAMIC_DRAW);
}

// only updates offsets for now
void UpdateDynamicMesh(MeshHandle handle, const std::vector<u32> &indices, Mesh *mesh)
{
  auto &va = sVertexArrays[handle];
  glBindBuffer(GL_ARRAY_BUFFER, va.vertexBuffers[2].handle);
  for (u32 i : indices)
  {
    glBufferSubData(
        GL_ARRAY_BUFFER, i * VEC3_SIZE * sizeof(f32), VEC3_SIZE * sizeof(f32),
        &mesh->mOffsets.AccessBuffer(i * VEC3_SIZE));
  }
}

NODISCARD MeshHandle SubmitStaticMesh(Mesh *mesh, ShaderProgram program)
{
  return StoreMesh(mesh, GL_ARRAY_BUFFER, program, GL_STATIC_DRAW);
}

void RemoveMesh(MeshHandle handle)
{
  auto &va = sVertexArrays[handle];
  va.Destroy();
}

void Draw(MeshHandle handle, const std::vector<ShaderData> &data, DrawMode drawMode)
{
  auto &va = sVertexArrays[handle];
  sShaders[va.program].Bind();
  glBindVertexArray(va.handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, va.indexBuffer.handle);
  if (drawMode == DrawMode::TRIANGLES)
  {
    glDrawElements(GL_TRIANGLES, va.indexBuffer.size, GL_UNSIGNED_INT, (void *)0);
  }
  else
  {
    glDrawElements(GL_LINES, va.indexBuffer.size, GL_UNSIGNED_INT, (void *)0);
  }
}

void SwapBuffers()
{
  glfwSwapBuffers(sWindow);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace Renderer
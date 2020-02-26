#include "RendererBackend.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"
#include "camera.h"
#include "mesh.h"
#include "meshLibrary.h"
#include "shader.h"
#include "shaderLibrary.h"
#include "texture.h"
#include "textureLibrary.h"
#include "window.h"

#include <GLFW/glfw3.h>

namespace Renderer
{
static void ErrorCallback(
    GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char *message,
    const void *userParam)
{
  printf("%s\n", message);
}

Window *InitAPI(const s32 width, const s32 height, const char *windowName)
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

  glfwMakeContextCurrent(window->mGLWindow);
  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    fprintf(stderr, "glew error: %s\n", glewGetErrorString(err));
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSwapInterval(1);
  glEnable(GL_DEPTH_TEST);
  //  glCullFace(GL_BACK);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glPolygonMode(GL_FRONT, GL_FILL);
  glClearDepth(4.0);
  glDepthFunc(GL_LESS);

  glDebugMessageCallback(ErrorCallback, nullptr);
  // glEnable(GL_DEBUG_OUTPUT);
  return window;
}

RendererBackend::RendererBackend(Window *window) :
    mWindow(window), mShaderLibrary(new ShaderLibrary()), mTextureLibrary(new TextureLibrary()),
    mMeshLibrary(new MeshLibrary())
{
  glEnable(GL_PROGRAM_POINT_SIZE);
  std::vector<std::string> shaderPaths(
      {"../shaders/flatColorShader.vert", "../shaders/flatColorShader.frag"});
  mShaderLibrary->Add(shaderPaths);
  mShaderLibrary->Add(
      std::vector<std::string>({"../shaders/pointShader.vert", "../shaders/pointShader.frag"}));
  mShaderLibrary->Add(
      std::vector<std::string>({"../shaders/textureShader.vert", "../shaders/textureShader.frag"}));
  mShaderLibrary->Add(
      std::vector<std::string>({"../shaders/flatShader.vert", "../shaders/flatShader.frag"}));
}

u32 RendererBackend::SubmitMesh(Mesh *mesh)
{
  return mMeshLibrary->Insert(mesh);
}
u32 RendererBackend::SubmitTexture(Texture2D *texture)
{
  GLTexture2D glTexture2D;
  glTexture2D.Create(texture);
  return mTextureLibrary->Add(glTexture2D);
}

void RendererBackend::Draw()
{
  for (const auto &command : mCommandQueue)
  {

    auto *shader = GetShader(command.mType, true);
    shader->SetShaderData(command.mShaderData);
    switch (command.mType)
    {
    case CommandType::DrawSolid:
    case CommandType::DrawSolidFlatShade:
    {
      auto *shader = GetShader(command.mType, true);

      // shader->SetUniform3F("color", command.mColor);
      // shader->SetUniformMat4("camera", camera);
      // shader->SetUniformMat4("projection", projection);
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_TRIANGLES, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawLine:
    {
      auto *shader = GetShader(command.mType, true);
      // shader->SetUniformMat4("camera", camera);
      // shader->SetUniformMat4("projection", projection);
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_LINES, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawPoints:
    {
      auto *shader = GetShader(command.mType, true);
      // shader->SetUniformMat4("camera", camera);
      // shader->SetUniformMat4("projection", projection);
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_POINTS, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawTextured:
    {
      auto *shader = GetShader(command.mType, true);
      // shader->SetUniformMat4("camera", camera);
      // shader->SetUniformMat4("projection", projection);
      auto texture = mTextureLibrary->GetTexture(command.mTextureHandle);
      texture.Bind();
    }
    break;
    // TODO: consider removing this command and adding a member function instead
    case CommandType::UpdateMesh:
      mMeshLibrary->Update(command.mMesh, command.mMeshHandle);
      break;
    case CommandType::ClearDepthBuffer:
      glClear(GL_DEPTH_BUFFER_BIT);
      break;
    }
  }
}

void RendererBackend::UpdateScreen()
{
  glfwSwapBuffers(mWindow->mGLWindow);
}

void RendererBackend::SetClearColor(const glm::vec4 &color)
{
  glClearColor(color.r, color.g, color.b, color.a);
}

void RendererBackend::ClearScreen()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererBackend::RemoveMesh(u32 handle)
{
  mMeshLibrary->Remove(handle);
}

// Private
Shader *RendererBackend::GetShader(const CommandType type, bool bind)
{
  Shader *shader = nullptr;
  switch (type)
  {
  case CommandType::DrawSolid:
  case CommandType::DrawLine:
    shader = mShaderLibrary->GetProgram("flatColorShader");
    break;
  case CommandType::DrawSolidFlatShade:
    shader = mShaderLibrary->GetProgram("flatShader");
    break;
  case CommandType::DrawPoints:
    shader = mShaderLibrary->GetProgram("pointShader");
    break;
  case CommandType::DrawTextured:
    shader = mShaderLibrary->GetProgram("texturedShader");
    break;
  default:
    assert(0);
  }
  if (bind)
  {
    shader->Bind();
  }
  return shader;
}
IndexBuffer RendererBackend::GetBuffersAndBind(const u32 handle)
{
  auto [vao, ibo] = (*mMeshLibrary)[handle];
  vao.Bind();
  ibo.Bind();
  return ibo;
}

} // namespace Renderer

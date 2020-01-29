#include "renderer.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"
#include "camera.h"
#include "meshLibrary.h"
#include "shader.h"
#include "shaderLibrary.h"
#include "texture.h"
#include "textureLibrary.h"
#include "window.h"

#include <GLFW/glfw3.h>

namespace ren
{
Window *InitAPI(s32 width, s32 height, const char *windowName)
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
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glClearDepth(4.0);
  glDepthFunc(GL_LESS);
  return window;
}

void InitUI(Window *window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui_ImplGlfw_InitForOpenGL(window->mGLWindow, true);
  ImGui_ImplOpenGL3_Init("#version 150");
}

} // namespace ren

Renderer::Renderer(Window *window) :
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
}

u32 Renderer::SubmitMesh(Mesh *mesh)
{
  return mMeshLibrary->Insert(mesh);
}
u32 Renderer::SubmitTexture(Texture2D *texture)
{
  GLTexture2D glTexture2D;
  glTexture2D.Create(texture);
  return mTextureLibrary->Add(glTexture2D);
}

void Renderer::Draw(Camera *camera, const glm::mat4 &projection)
{
  for (const auto &command : mCommandQueue)
  {
    switch (command.mType)
    {
    case CommandType::DrawSolid:
    {
      auto *shader = GetShader(command.mType, true);
      shader->SetUniform3F("color", command.mColor);
      shader->SetUniformMat4("camera", camera->CalculateMatrix());
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_TRIANGLES, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawLine:
    {
      auto *shader = GetShader(command.mType, true);
      shader->SetUniformMat4("camera", camera->CalculateMatrix());
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_LINES, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawPoints:
    {
      auto *shader = GetShader(command.mType, true);
      shader->SetUniformMat4("camera", camera->CalculateMatrix());
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_POINTS, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawTextured:
    {
      auto *shader = GetShader(command.mType, true);
      shader->SetUniformMat4("camera", camera->CalculateMatrix());
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

void Renderer::UpdateScreen()
{
  glfwSwapBuffers(mWindow->mGLWindow);
}

void Renderer::SetClearColor(const glm::vec4 &color)
{
  glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::ClearScreen()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Shader *Renderer::GetShader(const CommandType type, bool bind)
{
  Shader *shader = nullptr;
  switch (type)
  {
  case CommandType::DrawSolid:
  case CommandType::DrawLine:
    shader = mShaderLibrary->GetProgram("flatColorShader");
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
IndexBuffer Renderer::GetBuffersAndBind(const u32 handle)
{
  auto [vao, ibo] = (*mMeshLibrary)[handle];
  vao.Bind();
  ibo.Bind();
  return ibo;
}

#define GLM_ENABLE_EXPERIMENTAL
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "common.h"
#include "ecs/ecs.h"
#include "renderer/camera.h"
#include "renderer/mesh.h"
#include "renderer/renderer.h"
#include "window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

constexpr s32 width = 720;
constexpr s32 height = 480;

void ErrorCallback(
    GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char *message,
    const void *userParam)
{
  printf("%s\n", message);
}

Window *InitGL()
{
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to init GLFW\n");
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  //   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
  //      GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  Window *window = new Window(width, height);

  glfwMakeContextCurrent(window->mGLFWWindow);
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

void InitIMGUI(Window *window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui_ImplGlfw_InitForOpenGL(window->mGLFWWindow, true);
  ImGui_ImplOpenGL3_Init("#version 150");
}

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  Window *window = InitGL();
  // InitIMGUI(window);
  printf("%s\n", glGetString(GL_VERSION));
  glDebugMessageCallback(ErrorCallback, nullptr);
  glEnable(GL_DEBUG_OUTPUT);
  f64 lastTime = glfwGetTime();

  Renderer renderer(window);

  Mesh mesh;
  mesh.mVertecies.assign({-0.8f, 0.0f, -0.8f, 0.8f, 0.0f, -0.0f, 0.8f, 0.8f, -0.8f});
  mesh.mIndecies.assign({0, 1, 2});
  auto handle = renderer.SubmitMesh(&mesh);

  Camera camera;

  Command command(CommandType::DrawSolid, handle);
  command.mColor = {1.0f, 0.0f, 1.0f};
  renderer.SubmitCommand(command);
  Command clearCommand(CommandType::ClearDepthBuffer);
  renderer.SubmitCommand(clearCommand);
  Command commandPoint(CommandType::DrawPoints, handle);
  renderer.SubmitCommand(commandPoint);

  while (!window->ShouldClose())
  {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer.Draw(&camera, glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.5f, 100.0f));
    renderer.UpdateScreen();
    f64 currentTime = glfwGetTime();
    f64 delta = (currentTime - lastTime);
    lastTime = glfwGetTime();
  }

  // ImGui_ImplOpenGL3_Shutdown();
  // ImGui_ImplGlfw_Shutdown();
  // ImGui::DestroyContext();

  return 0;
}

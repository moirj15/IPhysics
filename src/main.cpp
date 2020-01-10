#define GLM_ENABLE_EXPERIMENTAL
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "common.h"
#include "window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

constexpr s32 width = 1920;
constexpr s32 height = 1080;

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

  while (!window->ShouldClose())
  {
    glfwPollEvents();
    f64 currentTime = glfwGetTime();
    f64 delta = (currentTime - lastTime);
    lastTime = glfwGetTime();
  }

  // ImGui_ImplOpenGL3_Shutdown();
  // ImGui_ImplGlfw_Shutdown();
  // ImGui::DestroyContext();

  return 0;
}

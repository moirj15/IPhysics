#define GLM_ENABLE_EXPERIMENTAL
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "common.h"
#include "ecs/ecs.h"
#include "renderer/RendererBackend.h"
#include "renderer/camera.h"
#include "renderer/mesh.h"
#include "renderer/window.h"

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

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  // Window *window = Renderer::InitAPI(width, height, "IPhysics");
  // // InitIMGUI(window);
  // printf("%s\n", glGetString(GL_VERSION));
  // glDebugMessageCallback(ErrorCallback, nullptr);
  // glEnable(GL_DEBUG_OUTPUT);
  // f64 lastTime = glfwGetTime();
  //
  // Renderer::RendererBackend renderer(window);
  //
  // Mesh mesh;
  // mesh.mVertecies.assign({-0.8f, 0.0f, -0.8f, 0.8f, 0.0f, -0.0f, 0.8f, 0.8f, -0.8f});
  // mesh.mIndecies.assign({0, 1, 2});
  // auto handle = renderer.SubmitMesh(&mesh);
  //
  // Camera camera;
  //
  // Renderer::DrawCommand command(Renderer::CommandType::DrawSolid, handle);
  // command.mColor = {1.0f, 0.0f, 1.0f};
  // renderer.SubmitCommand(command);
  // Renderer::DrawCommand clearCommand(Renderer::CommandType::ClearDepthBuffer);
  // renderer.SubmitCommand(clearCommand);
  // Renderer::DrawCommand commandPoint(Renderer::CommandType::DrawPoints, handle);
  // renderer.SubmitCommand(commandPoint);
  //
  // while (!window->ShouldClose())
  // {
  //   glfwPollEvents();
  //   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //   renderer.Draw(&camera, glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.5f, 100.0f));
  //   renderer.UpdateScreen();
  //   f64 currentTime = glfwGetTime();
  //   f64 delta = (currentTime - lastTime);
  //   lastTime = glfwGetTime();
  // }
  //
  // // ImGui_ImplOpenGL3_Shutdown();
  // // ImGui_ImplGlfw_Shutdown();
  // // ImGui::DestroyContext();

  return 0;
}

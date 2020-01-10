#include "window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
Window::Window(s32 width, s32 height) : mWidth(width), mHeight(height), mGLFWWindow(nullptr)
{
  mGLFWWindow = glfwCreateWindow(mWidth, mHeight, "DView", nullptr, nullptr);
  if (!mGLFWWindow)
  {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(mGLFWWindow);

  glViewport(0, 0, mWidth, mHeight);
}

Window::~Window()
{
  glfwDestroyWindow(mGLFWWindow);
}
bool Window::ShouldClose()
{
  return glfwWindowShouldClose(mGLFWWindow);
}

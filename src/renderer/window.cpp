#include "window.h"

#include <GLFW/glfw3.h>
#include <assert.h>

Window::Window(s32 width, s32 height, const char *title) :
    mWidth(width), mHeight(height), mGLWindow(nullptr)
{
  mGLWindow = glfwCreateWindow(mWidth, mHeight, title, nullptr, nullptr);
  if (!mGLWindow)
  {
    printf("FAILED TO CREATE GLFW WINDOW\n");
    glfwTerminate();
    assert(0);
  }
  glfwMakeContextCurrent(mGLWindow);
  glViewport(0, 0, mWidth, mHeight);
}

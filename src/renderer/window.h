#pragma once
#include "../common.h"

struct GLFWwindow;

struct Window
{
  s32 mWidth;
  s32 mHeight;
  GLFWwindow *mGLWindow;

  Window(s32 width, s32 height, const char *title);
};

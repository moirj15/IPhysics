#pragma once

#include "common.h"

struct GLFWwindow;
struct Window
{
  s32 mWidth;
  s32 mHeight;
  GLFWwindow *mGLFWWindow;

  Window(s32 width, s32 height);
  ~Window();

  bool ShouldClose();
};

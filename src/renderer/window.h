#pragma once
#include "../common.h"

struct GLFWwindow;

struct Window
{
  s32 mWidth;
  s32 mHeight;
  GLFWwindow *mGLWindow;

  /**
   * \brief: Construct a window
   * \param width: The width of the window in pixels.
   * \param height: The height of the window in pixels.
   * \param title: The title of the window.
   */
  Window(s32 width, s32 height, const char *title);
  ~Window();

  bool ShouldClose();
  NODISCARD s32 GetWidth() const
  {
    return mWidth;
  }
  NODISCARD s32 GetHeight() const
  {
    return mHeight;
  }
};

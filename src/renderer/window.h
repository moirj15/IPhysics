#pragma once
#include "../common.h"

#include <windows.h>

struct Window
{
  s32 m_width;
  s32 m_height;
  const char *m_title;
  HWND m_windowHandle;

  using MessageHandler = LRESULT (*)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  Window(s32 width, s32 height, const char *title) :
      m_width(width), m_height(height), m_title(title), m_windowHandle(0)
  {
  }

  void Init(MessageHandler messageHandler, HINSTANCE instanceHandle, int showCmd);
  void Destroy();
};

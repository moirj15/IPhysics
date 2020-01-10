#include "window.h"

void Window::Init(MessageHandler messageHandler, HINSTANCE instanceHandle, int showCmd)
{
  WNDCLASS wc = {};
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = messageHandler;
  wc.hInstance = instanceHandle;
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszClassName = m_title;

  if (!RegisterClass(&wc))
  {
    MessageBox(0, "RegisterClass Failed", 0, 0);
    return;
  }
  m_windowHandle = CreateWindow(
      m_title, m_title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_width, m_height, 0, 0,
      instanceHandle, 0);
  if (!m_windowHandle)
  {
    MessageBox(0, "CreateWindow Failed", 0, 0);
    return;
  }
  ShowWindow(m_windowHandle, true);
  UpdateWindow(m_windowHandle);
}

void Window::Destroy()
{
}

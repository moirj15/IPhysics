#include "Input/InputManager.h"

// #include <GLFW/glfw3.h>

namespace Input
{

static GLFWwindow *sWindow;
static bool sKeys[256];
static glm::dvec2 sMousePosition;

// static void KeyCallback(GLFWwindow *window, s32 key, s32 scancode, s32 action, s32 mods)
// {
//   sKeys[key] = action != GLFW_RELEASE;
// }
//
// void Init(GLFWwindow *window)
// {
//   sWindow = window;
//   // glfwSetKeyCallback(sWindow, KeyCallback);
//   // glfwGetCursorPos(sWindow, &sMousePosition.x, &sMousePosition.y);
// }
//
// void Update()
// {
//   glfwPollEvents();
// }
//
// bool isKeyPressed(const u32 key)
// {
//   return sKeys[key];
// }
//
// glm::dvec2 GetMouseDelta()
// {
//   glm::dvec2 currPos;
//   // glfwGetCursorPos(sWindow, &currPos.x, &currPos.y);
//   auto delta = sMousePosition - currPos;
//   sMousePosition = currPos;
//   return delta;
// }

} // namespace Input

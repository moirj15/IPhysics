#pragma once

#include <Common.h>

#include <glm/vec2.hpp>

struct GLFWwindow;

namespace Input
{

void Init(GLFWwindow *window);
void Update();
NODISCARD bool isKeyPressed(const u32 key);
NODISCARD glm::dvec2 GetMouseDelta();

} // namespace Input
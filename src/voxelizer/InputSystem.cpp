#include "InputSystem.hpp"

#include "../third_party/imgui/imgui.h"

#include <Common.h>
#include <SDL2/SDL.h>

void InputSystem::BuildEvents()
{
#if 0
    glfwPollEvents();
#endif
    auto &io = ImGui::GetIO();
    auto *keys = SDL_GetKeyboardState(nullptr);
    if (io.WantCaptureKeyboard) {
        return;
    }
    f32 boost = 1.0f;
    auto key = e.key.keysym.sym;
    if ((SDL_GetModState() & SDLK_LSHIFT) && !io.WantCaptureKeyboard) {
        boost = 10.0f;
    }
    if (keys[SDL_SCANCODE_W]) {
        m_camera.Move(glm::vec3(0.0f, 0.0f, 1.0f) * boost * io.DeltaTime);
    }
    if (keys[SDL_SCANCODE_S]) {
        m_camera.Move(glm::vec3(0.0f, 0.0f, -1.0f) * boost * io.DeltaTime);
    }
    if (keys[SDL_SCANCODE_A]) {
        m_camera.Move(glm::vec3(-1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
    }
    if (keys[SDL_SCANCODE_D]) {
        m_camera.Move(glm::vec3(1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
    }
    if (keys[SDL_SCANCODE_E]) {
        m_camera.Move(glm::vec3(0.0f, 1.0f, 0.0f) * boost * io.DeltaTime);
    }
    if (keys[SDL_SCANCODE_Q]) {
        m_camera.Move(glm::vec3(0.0f, -1.0f, 0.0f) * boost * io.DeltaTime);
    }
    if (!io.WantCaptureMouse && io.MouseDown[0]) {
        f32 screenWidth = f32(m_window.width);
        f32 screenHeight = f32(m_window.height);
        glm::vec2 mouseDelta((screenWidth / 2.0f) - io.MousePos.x, (screenHeight / 2.0f) - io.MousePos.y);
        m_camera.Rotate(mouseDelta * io.DeltaTime * 10.0f);
    }
}

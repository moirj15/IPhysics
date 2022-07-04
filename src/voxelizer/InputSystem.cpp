#include "InputSystem.hpp"

#include "../third_party/imgui/backends/imgui_impl_sdl.h"
#include "System.hpp"

#include <Common.h>
#include <SDL.h>

void InputSystem::BuildEvents()
{
    std::vector<System::Event> event_queue;
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        auto &io = ImGui::GetIO();
        auto *keys = SDL_GetKeyboardState(nullptr);
        if (io.WantCaptureKeyboard) {
            return;
        }
        if (event.type == SDL_QUIT) {
            return;
        }
        if ((SDL_GetModState() & KMOD_LSHIFT) && !io.WantCaptureKeyboard) {
            event_queue.emplace_back(System::Event::MoveBoost);
        }
        if (keys[SDL_SCANCODE_W]) {
            event_queue.emplace_back(System::Event::MoveForward);
        }
        if (keys[SDL_SCANCODE_S]) {
            event_queue.emplace_back(System::Event::MoveBackwards);
        }
        if (keys[SDL_SCANCODE_A]) {
            event_queue.emplace_back(System::Event::MoveLeft);
        }
        if (keys[SDL_SCANCODE_D]) {
            event_queue.emplace_back(System::Event::MoveRight);
        }
        if (keys[SDL_SCANCODE_E]) {
            event_queue.emplace_back(System::Event::MoveUp);
        }
        if (keys[SDL_SCANCODE_Q]) {
            event_queue.emplace_back(System::Event::MoveDown);
        }
        if (!io.WantCaptureMouse && io.MouseDown[0]) {
            event_queue.emplace_back(System::Event::LeftClick);
        }
    }
    for (auto *subscriber : m_subscribers) {
        subscriber->EnqueueEvents(event_queue);
    }
}

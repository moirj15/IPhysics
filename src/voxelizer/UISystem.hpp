//
// Created by Jimmy on 2/9/2020.
//

#pragma once

#include "VoxelizerParameters.h"

#include "Common.h"
#include <optional>
#include <string>

struct SDL_Window;
class ResourceSystem;
class UISystem
{
    std::string m_mesh_file_name;
    Parameters mParameters;
    bool mSaveClicked;

    SDL_Window *m_window = nullptr;
    ResourceSystem *m_resource_system = nullptr;

public:
    UISystem(SDL_Window *window, ResourceSystem *resource_system);
    ~UISystem();

    void Update();
};

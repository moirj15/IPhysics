#pragma once

#include "Common.h"
#include "Objects.h"
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>
#include "Handle.hpp"


class System;

using MeshHandle = Handle<objs::Mesh>;

class ResourceSystem
{
    std::unordered_map<std::string, Handle<objs::Mesh>> m_mesh_cache;
    std::unordered_map<Handle<objs::Mesh>, objs::Mesh> m_meshes;

    std::vector<System *> m_mesh_listeners;

    u32 m_next_handle = 0;

public:
    ResourceSystem() = default;

    ResourceSystem(const ResourceSystem &) = delete;
    ResourceSystem(ResourceSystem &&) = delete;
    ResourceSystem &operator=(const ResourceSystem &) = delete;
    ResourceSystem &operator=(ResourceSystem &&) = delete;

    Handle<objs::Mesh> LoadMesh(const std::string &path);
    const objs::Mesh &GetMesh(MeshHandle handle) const { return m_meshes.at(handle); }

    void AddMeshListener(System *listener) { m_mesh_listeners.emplace_back(listener); }
};

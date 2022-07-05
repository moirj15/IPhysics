#pragma once

#include <Common.h>
#include <VoxelObjects/Objects.h>
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

template<typename T>
class Handle
{
    u32 m_handle = 0;

public:
    explicit Handle(u32 handle) : m_handle(handle) { asseret(handle != 0); }

    u32 Data() const { return m_handle; }
    bool IsValid() const { return m_handle != 0; }
};

class System;

class ResourceSystem
{
    std::unordered_map<std::string, objs::Mesh> m_mesh_cache;
    std::unordered_map<Handle<objs::Mesh>, objs::Mesh> m_meshes;

    std::vector<System *> m_mesh_listeners;

public:
    ResourceSystem() = default;

    ResourceSystem(const ResourceSystem &) = delete;
    ResourceSystem(ResourceSystem &&) = delete;
    ResourceSystem &operator=(const ResourceSystem &) = delete;
    ResourceSystem &operator=(ResourceSystem &&) = delete;

    Handle<objs::Mesh> LoadMesh(const std::string &path);

    void AddMeshListener(System *listener) { m_mesh_listeners.emplace_back(listener); }
};

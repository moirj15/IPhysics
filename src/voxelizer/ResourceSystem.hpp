#pragma once

#include "Common.h"
#include "Objects.h"
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

template<typename T>
class Handle
{
    u32 m_handle = 0;

public:
    Handle() = default;
    explicit Handle(u32 handle) : m_handle(handle) { assert(m_handle != 0); }
    Handle(const Handle &o) : m_handle(o.m_handle) { assert(m_handle != 0); }
    Handle(Handle &&o) noexcept : m_handle(o.m_handle) { assert(m_handle != 0); }

    bool operator==(const Handle &o) const { return m_handle == o.m_handle;}

    static Handle Invalid() { return Handle(); }

    [[nodiscard]] u32 Data() const { return m_handle; }
    [[nodiscard]] bool IsValid() const { return m_handle != 0; }


};

namespace std {

template<typename T>
struct hash<Handle<T>> {
    size_t operator()(const Handle<T> &handle) const {
        return hash<u32>()(handle.Data());
    }
};

}

class System;

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

    void AddMeshListener(System *listener) { m_mesh_listeners.emplace_back(listener); }
};

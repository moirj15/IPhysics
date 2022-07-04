#pragma once

#include <Common.h>
#include <VoxelObjects/Objects.h>
#include <string>
#include <unordered_map>

template<typename T>
class Handle
{
    u32 m_handle = 0;

public:
    explicit Handle(u32 handle) : m_handle(handle) {}

    u32 Data() const { return m_handle; }
};

class ResourceSystem
{
    std::unordered_map<std::string, objs::Mesh> m_mesh_cache;
    std::unordered_map<Handle<objs::Mesh>, objs::Mesh> m_meshes;

public:
};
